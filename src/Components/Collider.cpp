#include <limits>

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
	global_colliders.push_back(this);
	this->size = size;
}

Collider::Collider(
	Game& game, Object& object, Vector2 size, Layer layer
) : Component(game, object) {
	global_colliders.push_back(this);
	this->size = size;
	this->layer = layer;
}

Collider::~Collider() {
	global_colliders.push_back(this); // Remove from all colliders if deleted
}

Vector2 Collider::getSize() const {
	return this->size * object.getScale();
}

Layer Collider::getLayer() const {
	return this->layer;
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

Collider* Collider::getClosestCollider(Layer layer_mask) const {
	Collider* closest_collider = nullptr;
	float last_distance = std::numeric_limits<float>::max();
	Vector2 origin = this->object.getPosition();

	for (Collider* collider : global_colliders) {
		if (collider->layer != layer_mask) continue;

		Vector2 target_position = collider->object.getPosition();
		float new_distance = (target_position - origin).magnitude();
		if (new_distance >= last_distance) continue;
		
		closest_collider = collider;
		last_distance = new_distance;
	}
	return closest_collider;
}


//___________________
// Public functions

void Collider::fitInto(Collider* target) {
	Vector2 position = object.getPosition();
	Vector2 extent = getSize() * 0.5f;
	Bounds bounds = target->getBounds();

	// Clamp the position to fit into the target bounds
	position = Vector2::clamp(
		position, bounds.min + extent, bounds.max - extent
	);
	object.setPosition(position);
}

void Collider::update(float delta_time) {
}
