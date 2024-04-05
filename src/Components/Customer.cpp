#include <cmath>
#include <iostream>
#include <memory>
#include <string>

#include "Components/Customer.h"
#include "Components/CustomerAnimator.h"
#include "Components/Item.h"
#include "Core/Component.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Core/Utility/RandomGenerator.h"
#include "Core/Utility/Vector2.h"
#include "Data/Role.h"
#include "Factories/ItemFactory.h"
#include "Managers/CustomerBrain.h"
#include "Managers/DialogueManager.h"


//_______________
// Constructors

Customer::Customer(Game& game, Object& object) 
	: Component(game, object), dialogue_manager(game.getDialogueManager()) 
{
	animator = new CustomerAnimator(game, object);
	receive_region = game.getObject("receive_region");
}

Customer::~Customer() {
}


//__________
// Setters

void Customer::setCharacter(std::weak_ptr<CharacterData> character) {
	this->character = character;
}

void Customer::setCustomer(CustomerTrait trait, uint16_t funds) {
	this->trait = trait;
	this->funds = funds;
}


//___________________
// Public functions

void Customer::reactToPriceOffered(Item* item) {
	CustomerRequest request = game.getCustomerRequest();
	uint16_t offered_price = item->getCurrentPrice();

	// Check if in acceptable range
	if (CustomerBrain::isAcceptablePrice(*deal_data)) {
		if (CustomerBrain::willAcceptDeal(*deal_data)) {
			// Accept deal
			acceptDeal();
			return;
		}
		// Negotiate instead
		CustomerBrain::makeNewPriceOffer(*deal_data);
		return;
	}

	// Did customer already give a price offer?
	bool did_give_offer = item->getLastPrice() != 0u;
	if (!did_give_offer) {
		// Let customer place an offer
		CustomerBrain::makeNewPriceOffer(*deal_data);
		return;
	}

	if (CustomerBrain::willNegotiate(*deal_data)) {
		// Place new offer
		CustomerBrain::makeNewPriceOffer(*deal_data);
		return;
	}

	if (!CustomerBrain::willDeclineDeal(*deal_data)) {
		restateDeal();
		return;
	}

	// Decline deal and leave
	declineDeal();
}

void Customer::enter() {
	animator->setAnimation(CustomerAnimState::Entering);
	dialogue_manager.generateDialogue(Role::Merchant, "greeting");
}

void Customer::leave() {
	animator->setAnimation(CustomerAnimState::Leaving);
	game.setCustomerRequest(CustomerRequest::None);
}

void Customer::update(float delta_time) {
	// Only continue when ready for interactions
	if (animator->getAnimationState() != CustomerAnimState::Idling) return;
	
	// Generate a customer request if not already
	bool no_request = game.getCustomerRequest() == CustomerRequest::None;
	if (no_request) generateRequest();
}


//____________________
// Private functions

void Customer::generateRequest() {
	// TODO: Request based on inventory, needs and funds
	int random_number = utils::Random::generateInt(1, 3);

	CustomerRequest request = CustomerRequest::Selling;
	game.setCustomerRequest(request); // Do selling for now

	switch (request) {
		case CustomerRequest::Buying:
		{
			// TODO
			break;
		}
		case CustomerRequest::Selling:
		{
			Item* item = game.getItemFactory().generateRandomItem();
			Object& object = item->getObject();
			object.setParent(receive_region);
			deal_data = std::make_unique<DealData>(
				trait, request, funds, item, 
				utils::Random::generateFloat(0.5f, 1.f)
			);
			placeSellOffer();
			break;
		}
		case CustomerRequest::Trading:
		{
			// TODO
			break;
		}
	}
}

void Customer::placeSellOffer() {
	// TODO: Animate item sliding in

	int random_x = utils::Random::generateInt(
		-drop_range, drop_range
	);
	int random_y = utils::Random::generateInt(
		-drop_range, drop_range
	);
	deal_data->item->getObject().setLocalPosition(Vector2(random_x, random_y));
	dialogue_manager.generateDialogue(Role::Customer, "selling");
}


//____________________
// Private functions

void Customer::acceptDeal() {
	acceptable_price = deal_data->item->getCurrentPrice(); // New acceptable price
	dialogue_manager.generateDialogue(
		Role::Customer, "accept_deal"
	);
}

void Customer::declineDeal() {
	dialogue_manager.generateDialogue(
		Role::Customer, "decline_deal"
	);
}

void Customer::restateDeal() {
	deal_data->item->setCurrentPrice(acceptable_price);
	deal_data->item->setLatestOfferBy(Role::Customer);
	dialogue_manager.generateDialogue(
		Role::Customer, "restate_offer", std::to_string(acceptable_price)
	);
	deal_data->willingness_factor -= 0.1f; // Reduce willingness
}
