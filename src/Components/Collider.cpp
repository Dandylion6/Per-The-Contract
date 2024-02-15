#include "Components/Collider.h"
#include "Core/Component.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Core/Utility/Bounds.h"
#include "Core/Utility/Vector2.h"


//_______________
// Constructors

Collider::Collider(
	Game& game, Object& object, Vector2 size
) : Component(game, object) {
	this->size = size;
}

Collider::Collider(
	Game& game, Object& object, Vector2 size, Layer layer
) : Component(game, object) {
	this->size = size;
	this->layer = layer;
}

Collider::~Collider() {
}

Bounds Collider::getLocalBounds() const {
	return Bounds(
		Vector2(), this->size * object.getScale(),
		object.getAnchor()
	);
}

Bounds Collider::getBounds() const {
	return Bounds(
		object.getPosition(),
		this->size * object.getScale(),
		object.getAnchor()
	);
}


//___________________
// Public functions

void Collider::update(float delta_time) {
}
