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


//___________________
// Public functions

void Customer::setCharacter(CharacterData character) {
	this->character = character;
}

void Customer::update(float delta_time) {
}
