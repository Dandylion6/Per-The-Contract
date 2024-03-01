#include <memory>

#include "Components/Customer.h"
#include "Components/CustomerAnimator.h"
#include "Components/Renderers/SpriteRenderer.h"
#include "Core/Component.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Core/Utility/Math.h"


//_______________
// Constructors

Customer::Customer(
	Game& game, Object& object
) : Component(game, object) {
	animator = new CustomerAnimator(game, object);
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

void Customer::enter() {
	animator->setAnimation(CustomerAnimState::Entering);
}

void Customer::leave() {
	animator->setAnimation(CustomerAnimState::Leaving);
}

void Customer::update(float delta_time) {
	// Only continue when ready for interactions
	if (animator->getAnimationState() != CustomerAnimState::Idling) return;
}