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

void Customer::setFunds(uint16_t funds) {
	this->funds = funds;
}

void Customer::setCharacter(std::weak_ptr<CharacterData> character) {
	this->character = character;
}


//___________________
// Public functions

void Customer::addToInventory(std::string item) {
	inventory.push_back(item);
}

void Customer::enter() {
	animator->setAnimation(CustomerAnimState::Entering);
	dialogue_manager.generateDialogue(Role::Merchant, "greeting");
}

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
			placeSellOffer();
			dialogue_manager.generateDialogue(Role::Customer, "selling");
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
	// TODO: Choose items from customer inventory to sell.
	for (int i = 0; i < 1; i++) {
		Item* item = game.getItemFactory().generateRandomItem();
		Object& object = item->getObject();
		object.setParent(receive_region);

		// TODO: Choose an initial price
		item->setPrice(item->getData().market_value);

		// TODO: Animate item sliding in

		int random_x = utils::RandomGenerator::generateInt(
			-drop_range, drop_range
		);
		int random_y = utils::RandomGenerator::generateInt(
			-drop_range, drop_range
		);
		object.setLocalPosition(Vector2(random_x, random_y));
	}
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