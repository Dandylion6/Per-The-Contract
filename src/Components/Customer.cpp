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
#include "Managers/CustomerManager.h"
#include "Managers/DialogueManager.h"


//_______________
// Constructors

Customer::Customer(Game& game, Object& object) : Component(game, object) 
{
	animator = new CustomerAnimator(game, object);
	storage = game.getObject("storage");
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

	switch (trait) {
		case CustomerTrait::Assertive:
			std::cout << "[Trait]: Assertive" << std::endl;
			break;
		case CustomerTrait::OpenMinded:
			std::cout << "[Trait]: Open Minded" << std::endl;
			break;
		case CustomerTrait::Knowledgeable:
			std::cout << "[Trait]: Knowledgeable" << std::endl;
			break;
		case CustomerTrait::Frugal:
			std::cout << "[Trait]: Frugal" << std::endl;
			break;
		case CustomerTrait::Impulsive:
			std::cout << "[Trait]: Impulsive" << std::endl;
			break;
		case CustomerTrait::Trusting:
			std::cout << "[Trait]: Trusting" << std::endl;
			break;
	}
}


//___________________
// Public functions

void Customer::reactToPriceOffered(Item* item) {
	CustomerRequest request = game.getDealData()->request;
	uint16_t offered_price = item->getCurrentPrice();
	std::shared_ptr<DealData> deal_data = game.getDealData();

	// Check if in acceptable range
	if (CustomerBrain::isAcceptablePrice(*deal_data)) {
		handleAcceptableOffer();
		return;
	}

	// Did customer already give a price offer?
	bool had_initial_offer = item->getLastPrice() != 0u;
	if (!had_initial_offer) {
		// Let customer place an offer
		uint16_t new_offer = CustomerBrain::generatePriceOffer(*deal_data);
		negotiate(new_offer);
		return;
	}

	// React to player's unacceptable offer
	handleUnacceptableOffer();
}

void Customer::enter() {
	animator->setAnimation(CustomerAnimState::Entering);
	DialogueManager::getInstance().generateDialogue(Role::Merchant, "greeting");
	stated_request = false;
}

void Customer::leave() {
	// Remove item if selling
	std::shared_ptr<DealData> deal_data = game.getDealData();
	if (deal_data->request == CustomerRequest::Selling) {
		game.deleteObject(&deal_data->item->getObject());
	}
	game.setDealData(nullptr);
}

void Customer::update(float delta_time) {
	// Only continue when ready for interactions
	if (animator->getAnimationState() != CustomerAnimState::Idling) return;
	
	// State request
	if (stated_request) return;
	handleRequest(generateRequest());
	stated_request = true;
}


//____________________
// Private functions

CustomerRequest Customer::generateRequest() {
	int random_number = utils::Random::generateInt(0, 1); // TODO: Add contracts
	CustomerRequest request = static_cast<CustomerRequest>(random_number);
	std::vector<Item*> storage_items;
	for (Object* child : storage->getChildren()) {
		Item* item = child->getComponent<Item>();
		if (item == nullptr) continue;
		storage_items.push_back(item);
	}
	bool storage_empty = storage_items.size() == 0u;
	if (storage_empty && request == CustomerRequest::Buying) {
		return CustomerRequest::Selling;
	}
	return CustomerRequest::Selling;
}

void Customer::handleRequest(CustomerRequest request) {
	switch (request) {
		case CustomerRequest::Buying:
			// TEMP: Make proper buying offer system

			if (game.getDealData()->item != nullptr) {
				DialogueManager::getInstance().generateDialogue(
					Role::Customer, "buying_specific"
				);
				break;
			}
			DialogueManager::getInstance().generateDialogue(Role::Customer, "buying");
			break;
		case CustomerRequest::Selling:
			generateSellOffer();
			DialogueManager::getInstance().generateDialogue(Role::Customer, "selling");
			break;
	}

	// Additional dialogue based on trait
	if (trait == CustomerTrait::Frugal) {
		DialogueManager::getInstance().generateDialogue(Role::Customer, "frugal_trait");
	} else if (trait == CustomerTrait::Impulsive) {
		DialogueManager::getInstance().generateDialogue(Role::Customer, "impulsive_trait");
	}
}

void Customer::generateSellOffer() {
	Item* item = ItemFactory::getInstance().generateRandomItem();
	Object& object = item->getObject();
	object.setParent(receive_region);

	DealData* deal_data = new DealData(
		trait, CustomerRequest::Selling,
		funds, item, utils::Random::generateFloat(0.6f, 0.8f)
	);
	CustomerBrain::determinePerceivedPrice(*deal_data);
	game.setDealData(deal_data);

	int random_x = utils::Random::generateInt(-drop_range, drop_range);
	int random_y = utils::Random::generateInt(-drop_range, drop_range);
	deal_data->item->getObject().setLocalPosition(Vector2(random_x, random_y));
}

void Customer::determineBuyOffer() {
	std::vector<Item*> storage_items;
	for (Object* child : storage->getChildren()) {
		Item* item = child->getComponent<Item>();
		if (item == nullptr) continue;
		storage_items.push_back(item);
	}
	int random_index = utils::Random::randomIndex(storage_items.size());
	Item* item = storage_items.at(random_index);

	DealData* deal_data = new DealData(
		trait, CustomerRequest::Buying,
		funds, item, utils::Random::generateFloat(0.6f, 0.8f)
	);
	CustomerBrain::determinePerceivedPrice(*deal_data);
	game.setDealData(deal_data);
}

void Customer::handleAcceptableOffer() {
	std::shared_ptr<DealData> deal_data = game.getDealData();
	uint16_t new_offer = CustomerBrain::generatePriceOffer(*deal_data);
	uint16_t offered_price = deal_data->item->getCurrentPrice();
	bool willing_to_accept = CustomerBrain::willAcceptDeal(*deal_data);

	bool ideal_price_found = new_offer >= offered_price;
	if (deal_data->request == CustomerRequest::Selling) {
		ideal_price_found = new_offer <= offered_price;
	}

	if (ideal_price_found || willing_to_accept) {
		// Accept deal
		deal_data->acceptable_price = offered_price;
		DialogueManager::getInstance().generateDialogue(Role::Customer, "accept_deal");
		return;
	}

	negotiate(new_offer);
	deal_data->negotiability -= 0.06f;
	deal_data->willingness += 0.04f;
	return;
}

void Customer::handleUnacceptableOffer() {
	std::shared_ptr<DealData> deal_data = game.getDealData();
	CustomerBrain::playerOfferPenalty(*deal_data);
	if (CustomerBrain::willNegotiate(*deal_data)) {
		// Place new offer
		negotiate(CustomerBrain::generatePriceOffer(*deal_data));
		return;
	}

	if (!CustomerBrain::willDeclineDeal(*deal_data)) {
		// Repeat acceptable offer
		negotiate(deal_data->acceptable_price);
		deal_data->willingness -= 0.08f;
		return;
	}

	// Decline deal
	DialogueManager::getInstance().generateDialogue(Role::Customer, "decline_deal");
	leave();
	CustomerManager::getInstance().closeShop(false);
}

void Customer::negotiate(uint16_t new_offer) {
	std::shared_ptr<DealData> deal_data = game.getDealData();
	deal_data->item->setCurrentPrice(new_offer);
	deal_data->item->setLatestOfferBy(Role::Customer);

	// Repeat offer
	if (deal_data->acceptable_price == new_offer) {
		DialogueManager::getInstance().generateDialogue(
			Role::Customer, "restate_offer", std::to_string(new_offer)
		);
		return;
	}
	deal_data->acceptable_price = new_offer;
	DialogueManager::getInstance().generateDialogue(
		Role::Customer, "negotiate_offer", std::to_string(new_offer)
	);
}