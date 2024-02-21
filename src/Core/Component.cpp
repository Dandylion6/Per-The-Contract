#include "Core/Component.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"


//_______________
// Constructors

Component::Component(
	Game& game, Object& object
) : game(game), object(object) {
	object.addComponent(this);
}

Component::~Component() {
}


//__________
// Getters

Object& Component::getObject() const {
	return this->object;
}

bool Component::getEnabled() const {
	return this->enabled;
}


//__________
// Setters

void Component::setEnabled(bool enabled) {
	this->enabled = enabled;
}