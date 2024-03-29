#include <limits>
#include <list>
#include <vector>

#include "Components/Collider.h"
#include "Core/Component.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Core/Object.h"
#include "Core/Utility/Bounds.h"
#include "Core/Utility/Vector2.h"


std::list<Collider*> Collider::global_colliders;

//_______________
// Constructors

Collider::Collider(
	Game& game, Object& object, Vector2 size
) : Component(game, object) {
	global_colliders.push_back(this);
	this->size = size;
}

Collider::~Collider() {
	global_colliders.remove(this); // Remove from all colliders if deleted
}

Vector2 Collider::getSize() const {
	return this->size * object.getScale();
}


//__________
// Getters

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

Collider* Collider::getCollider(std::string name) {
	for (Collider* collider : global_colliders) {
		if (collider->getObject().getName() != name) continue;
		return collider; // Found collider with name
	}
	return nullptr;
}

Collider* Collider::getOverlapping() const {
	const std::list<Object*>& objects = game.getObjects();
	for (auto it = objects.rbegin(); it != objects.rend(); ++it) {
		Object* target_object = *it;
		for (Collider* collider : global_colliders) {
			if (collider == this) continue; // Can't overlap self
			// If the collder's object match
			if (&collider->getObject() != target_object) continue;

			Bounds self_bounds = this->getBounds();
			Bounds target_bounds = collider->getBounds();

			if (!self_bounds.overlaps(target_bounds)) continue;
			return collider;
		}
	}
	return nullptr;
}

Collider* Collider::getMostOverlapping(
	std::vector<Collider*> colliders
) const {
	Collider* most_overlapping = nullptr;
	float most_area = 0.f;
	for (Collider* collider : colliders) {
		Bounds self_bounds = this->getBounds();
		Bounds target_bounds = collider->getBounds();

		if (!self_bounds.overlaps(target_bounds)) continue;
		float area = self_bounds.getOverlapArea(target_bounds);

		if (area > most_area) {
			most_overlapping = collider;
			most_area = area;
		}
	}
	return most_overlapping;
}

bool Collider::pointHits(Vector2 point) {
	const std::list<Object*>& objects = game.getObjects();
	auto it = objects.rbegin();
	for (auto it = objects.rbegin(); it != objects.rend(); ++it) {
		Object* object = *it;
		Collider* collider = object->getComponent<Collider>();
		if (collider == nullptr) continue;

		if (!collider->getBounds().overlapsPoint(point)) continue;
		return collider == this;
	}
	return false;
}

void Collider::fitInto(Collider* target) {
	if (target == nullptr) return;

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