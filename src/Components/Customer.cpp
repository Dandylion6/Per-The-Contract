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


//___________________
// Public functions

void Customer::actOnPlayerOffer() {
}

void Customer::enter() {
	animator->setAnimation(CustomerAnimState::Entering);
	stated_request = false;
}

void Customer::leave() {
	// Remove item if selling
	std::shared_ptr<DealData> deal_data = game.getDealData();
	if (deal_data->request == CustomerRequest::Selling) {
		game.deleteObject(&deal_data->offered_item->getObject());
	}
	game.setDealData(nullptr);
}

void Customer::update(float delta_time) {
	// Only continue when ready for interactions
	if (animator->getAnimationState() != CustomerAnimState::Idling) return;
	
	

	// State request
	if (stated_request) return;
	handleRequest();
	stated_request = true;
}


//____________________
// Private functions

void Customer::handleRequest() {
	switch (game.getDealData()->request) {
		case CustomerRequest::Buying:
			DialogueManager::getInstance().generateDialogue(Role::Customer, "buying");
			break;
		case CustomerRequest::Selling:
			DialogueManager::getInstance().generateDialogue(Role::Customer, "selling");
			placeSellItem();
			break;
		case CustomerRequest::Contract:
			return;
	}
}

void Customer::placeSellItem() {
	Item* item = game.getDealData()->offered_item;
	item->getObject().setParent(receive_region);

	Vector2 drop_position = receive_region->getPosition();
	item->getObject().setPosition(drop_position - Vector2(0.f, 200.f));
	drop_position += utils::Random::randomRadius(drop_radius);
	item->move_to(drop_position);
}

void Customer::negotiate(uint16_t new_offer) {
}