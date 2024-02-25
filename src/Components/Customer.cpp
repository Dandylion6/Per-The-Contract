#include <memory>

#include "Components/Customer.h"
#include "Core/Component.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"


//_______________
// Constructors

Customer::Customer(
	Game& game, Object& object
) : Component(game, object) {
}

Customer::~Customer() {
}


//__________
// Setters

void Customer::setCharacter(std::weak_ptr<CharacterData> character) {
	this->character = character;
}

//___________________
// Public functions

void Customer::leave() {
}

void Customer::enter() {
}

void Customer::update(float delta_time) {
}
