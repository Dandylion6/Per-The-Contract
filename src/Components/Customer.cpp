#include <cmath>
#include <iostream>
#include <memory>
#include <string>

#include "Components/Customer.h"
#include "Components/CustomerAnimator.h"
#include "Components/Objects/Item.h"
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

Customer::Customer(Game& game, Object& object) : Component(game, object) 
{
	animator = new CustomerAnimator(game, object);
	storage = game.getObject("storage");
	receive_region = game.getObject("receive_region");
}

Customer::~Customer() {
}


//___________________
// Public functions

void Customer::actOnPlayerOffer() {
	std::shared_ptr<DealData> deal_data = game.getDealData();
	if (CustomerBrain::willAcceptDeal(*deal_data)) {
		// Accept the deal
		DialogueManager::getInstance().generateDialogue(Role::Customer, "accept_deal");
		uint16_t price = deal_data->offered_item->getCurrentPrice();
		deal_data->customer_accepted_price = std::make_unique<uint16_t>(price);
		return;
	}
}

void Customer::dropCash(uint16_t value) {
	for (Cash* cash : CashFactory::getInstance().createCash(value)) {
		Object& cash_object = cash->getObject();
		cash_object.setParent(receive_region);
		Vector2 drop_position = receive_region->getPosition();
		cash->getObject().setPosition(drop_position - Vector2(0.f, 150.f));
		drop_position += utils::Random::randomRadius(drop_radius);
		cash->move_to(drop_position);
	}
}

void Customer::enter() {
	animator->setAnimation(CustomerAnimState::Entering);
}

void Customer::leave() {
	animator->setAnimation(CustomerAnimState::None);
}

void Customer::update(float delta_time) {
	// Only continue when ready for interactions
	if (animator->getAnimationState() != CustomerAnimState::Idling) return;

	// State request
	std::shared_ptr<DealData> deal_data = game.getDealData();
	if (deal_data->deal_started) return;
	handleRequest();
	deal_data->deal_started = true;
}


//____________________
// Private functions

void Customer::handleRequest() {
	switch (game.getDealData()->request) {
		case CustomerRequest::Buying: {
			ItemData& item_data = ItemFactory::getInstance().getItemData(game.getDealData()->request_id);
			DialogueManager::getInstance().generateDialogue(Role::Customer, "buying", item_data.name);
			break;
		}
		case CustomerRequest::Selling: {
			DialogueManager::getInstance().generateDialogue(Role::Customer, "selling");
			placeSellItem();
			break;
		}
		case CustomerRequest::Contract: {
			return;
		}
	}
}

void Customer::placeSellItem() {
	Item* item = game.getDealData()->offered_item;
	item->getObject().setParent(receive_region);

	Vector2 drop_position = receive_region->getPosition();
	item->getObject().setPosition(drop_position - Vector2(0.f, 150.f));
	drop_position += utils::Random::randomRadius(drop_radius);
	item->move_to(drop_position);
}

void Customer::negotiate(uint16_t new_offer) {
}