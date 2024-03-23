#include <cstdlib>
#include <ctime> 
#include <memory>
#include <string>

#include "Components/Customer.h"
#include "Components/CustomerAnimator.h"
#include "Components/Renderers/SpriteRenderer.h"
#include "Core/Component.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Core/Utility/Math.h"
#include "Factories/ItemFactory.h"


//_______________
// Constructors

Customer::Customer(
	Game& game, Object& object
) : Component(game, object) {
	animator = new CustomerAnimator(game, object);
	srand(time(nullptr)); // seed the number generator
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
}

void Customer::generateRequest() {
	// TODO: Request based on inventory, needs and funds
	int random_number = rand() % 3; // Generates random transaction type
	request_type = new RequestType;

	*request_type = static_cast<RequestType>(random_number);
	*request_type = RequestType::Sell; // Do selling for now

	switch (*request_type) {
		case Buy:
		{
			// TODO
			break;
		}
		case Sell:
		{
			placeSellOffer();
			break;
		}
		case Trade:
		{
			// TODO
			break;
		}
	}
}

void Customer::placeSellOffer() {
	game.getItemFactory().generateRandomItem();
}

void Customer::leave() {
	animator->setAnimation(CustomerAnimState::Leaving);
	delete request_type;
}

void Customer::update(float delta_time) {
	// Only continue when ready for interactions
	if (animator->getAnimationState() != CustomerAnimState::Idling) return;
	
	// Generate a customer request if not already
	if (request_type == nullptr) generateRequest();
}