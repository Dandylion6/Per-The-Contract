#include <memory>

#include "SFML/Window/Mouse.hpp"

#include "Components/Collider.h"
#include "Components/Drag.h"
#include "Core/Component.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Core/Utility/Vector2.h"

//_______________
// Constructors

Drag::Drag(
	Game& game, Object& object,Collider& collider
) : 
	Component(game, object), 
	collider(collider) 
{
	// Get region colliders
	storage_region = Collider::getCollider("storage");
	send_region = Collider::getCollider("send_region");
	receive_region = Collider::getCollider("receive_region");
	
	// Create drag limit bounds
	Vector2 extent = collider.getSize() * 0.5f;
	Vector2 window_size = game.getWindow().getSize();
	drag_bounds = Bounds(extent, window_size - extent);

	// Make sure to not grab when spawned
	drag_pressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
}

Drag::~Drag() {
}


//___________________
// Public functions

void Drag::move_to(Vector2 position) {
	object.setZIndex(-1);
	begin_position = std::make_unique<Vector2>(object.getPosition());
	move_position = std::make_unique<Vector2>(position);
	time_moved = 0.f;
}

void Drag::update(float delta_time) {
	if (move_position != nullptr) {
		Vector2 position = Vector2::outExpo(
			*begin_position, *move_position, time_moved / move_time
		);
		object.setPosition(position);

		if (time_moved >= move_time) {
			move_position.release();
			time_moved = 0.f;
			object.setZIndex(0);
		}
		time_moved += delta_time;
		return;
	}

	Vector2 mouse_position = sf::Mouse::getPosition();
	Bounds bounds = collider.getBounds();

	bool button_pressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	bool state_changed = button_pressed != drag_pressed;
	drag_pressed = button_pressed;

	bool mouse_in_bounds = bounds.overlapsPoint(mouse_position);
	bool drag_start = state_changed && drag_pressed && mouse_in_bounds;
	bool drag_end = state_changed && !drag_pressed && drag_data.is_dragging;

	if (drag_start) {
		grab(mouse_position);
		return;
	}

	if (drag_pressed && drag_data.is_dragging) {
		drag(mouse_position, delta_time);
		return;
	}
	
	if (drag_end) {
		drop(mouse_position);
	}
}

void Drag::grab(Vector2& mouse_position) {
	if (!collider.pointHits(mouse_position)) return; 
	drag_data.is_dragging = true;
	drag_data.grab_offset = object.getPosition() - mouse_position;

	object.pushToFront();
	object.setScale(Vector2::scale(1.06f));

	// Update dragging behaviour
	updateDroppableRegions();
	Collider* new_region = collider.getMostOverlapping(drag_data.droppable_regions);
	if (new_region != nullptr) drag_data.current_region = new_region;
	updateRegionLock();
}

void Drag::drag(Vector2& mouse_position, float delta_time) {
	Vector2 target_position = drag_data.grab_offset + mouse_position;

	// Confine to region if locked
	if (drag_data.is_region_locked) {
		object.setPosition(target_position);
		collider.fitInto(drag_data.current_region);
		return;
	}

	// Clamp to the screen size
	target_position = Vector2::clamp(
		target_position, drag_bounds.min, drag_bounds.max
	);
	object.setPosition(target_position);
}

void Drag::drop(Vector2& mouse_position) {
	drag_data.is_dragging = false;
	object.setScale(Vector2::scale(1.f));
	object.setRotation(0.f);
	confineToRegion();
	// Check when waiting for item cleanup if next customer can come in
}

void Drag::confineToRegion() {
	if (drag_data.is_region_locked) return; // No need if locked
	Collider* fit_to = collider.getMostOverlapping(drag_data.droppable_regions);
	drag_data.current_region = fit_to == nullptr ? drag_data.current_region : fit_to;
	collider.fitInto(drag_data.current_region);
	object.setParent(&drag_data.current_region->getObject()); // Set region as parent
}
