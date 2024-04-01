#include <cmath>
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
#include "Managers/DialogueManager.h"


//_______________
// Constructors

Customer::Customer(
	Game& game, Object& object
) : 
	Component(game, object), 
	dialogue_manager(game.getDialogueManager()) 
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

void Customer::setCustomer(
	CustomerTrait trait, uint16_t funds, float willingness_factor
) {
	this->trait = trait;
	this->funds = funds;
	this->negotiability_factor = negotiability_trait.find(trait)->second;
	this->acceptable_range_factor = acceptable_range_trait.find(trait)->second;
	this->willingness_factor = willingness_factor;
}


//___________________
// Public functions

void Customer::reactToPriceOffered(Item* item) {
	CustomerRequest request = game.getCustomerRequest();
	uint16_t offered_price = item->getCurrentPrice();

	// Check if in acceptable range
	bool in_price_range = isAcceptablePrice(offered_price);
	if (in_price_range) {
		if (willAcceptDeal()) {
			// Accept deal
			return;
		}
		// Negotiate instead
		placeNewPriceOffer(item);
		return;
	}

	// Did customer already give a price offer?
	bool did_give_offer = item->getLastPrice() != 0u;
	if (!did_give_offer) {
		// Let customer place an offer
		placeNewPriceOffer(item);
		return;
	}

	
	

	// Otherwise restate last acceptable price 
	// Decline outright if willingness is too low or can't continue
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
	int random_number = utils::RandomGenerator::generateInt(1, 3);

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
			Item* to_sell = generateItem();
			placeSellOffer(to_sell);
			break;
		}
		case CustomerRequest::Trading:
		{
			// TODO
			break;
		}
	}
}

Item* Customer::generateItem() {
	Item* item = game.getItemFactory().generateRandomItem();
	Object& object = item->getObject();
	object.setParent(receive_region);

	// TODO: Generate a perceived item value
	// if knowledeable will reflect more closely to market value
	perceived_item_value = item->getData().market_value;
	return item;
}

void Customer::placeSellOffer(Item* to_sell) {
	// TODO: Animate item sliding in

	int random_x = utils::RandomGenerator::generateInt(
		-drop_range, drop_range
	);
	int random_y = utils::RandomGenerator::generateInt(
		-drop_range, drop_range
	);
	to_sell->getObject().setLocalPosition(Vector2(random_x, random_y));
	dialogue_manager.generateDialogue(Role::Customer, "selling");
}

void Customer::placeNewPriceOffer(Item* item) {
}

bool Customer::isAcceptablePrice(uint16_t offered_price) const {
	CustomerRequest request = game.getCustomerRequest();
	uint16_t range = perceived_item_value * acceptable_range_factor;
	if (request == CustomerRequest::Selling) {
		uint16_t minimum = perceived_item_value - range;
		return offered_price >= minimum;
	} else {
		uint16_t maximum = perceived_item_value + range;
		return offered_price <= maximum;
	}
}

bool Customer::willAcceptDeal() const {
	// Certain customers might be more willing to accept or negotiate futher
	float accept_factor = 1.f - (negotiability_factor * 0.6f);
	float accept_value = utils::RandomGenerator::generateFloat(0.f, accept_factor);

	float acceptance_threshold = negotiability_factor * 0.4f;
	// If willingness is low they are more likely to accept instead of continuing
	acceptance_threshold -= willingness_factor * 0.3f;
	return accept_value > acceptance_threshold;
}