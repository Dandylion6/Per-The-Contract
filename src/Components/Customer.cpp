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

void Customer::actOnPlayerOffer() {
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
			DialogueManager::getInstance().generateDialogue(Role::Customer, "buying");
			break;
		case CustomerRequest::Selling:
			DialogueManager::getInstance().generateDialogue(Role::Customer, "selling");
			break;
	}
}

void Customer::negotiate(uint16_t new_offer) {
}