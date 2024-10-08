#define _USE_MATH_DEFINES

#include <memory>

#include "SFML/Window/Mouse.hpp"

#include "Components/Collider.h"
#include "Components/Drag.h"
#include "Components/Renderers/OutlineRenderer.h"
#include "Core/Component.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Core/Utility/Bounds.h"
#include "Core/Utility/Math.h"
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

	createOutline();

	// So isn't grabbed on spawn
	drag_pressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
}

Drag::~Drag() {
}


//___________________
// Public functions

void Drag::move_to(Vector2 position) {
	if (object.getPosition() == position) return;
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
		}
		time_moved += delta_time;
	} else {
		Vector2 mouse_position = sf::Mouse::getPosition();
		Bounds bounds = collider.getBounds();

		bool button_pressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
		bool state_changed = button_pressed != drag_pressed;
		drag_pressed = button_pressed;

		bool mouse_in_bounds = bounds.overlapsPoint(mouse_position);
		bool is_target = collider.pointHits(mouse_position);
		bool drag_start = state_changed && drag_pressed && mouse_in_bounds;
		bool drag_end = state_changed && !drag_pressed && drag_data.is_dragging;
		is_hovering = mouse_in_bounds && is_target && !button_pressed;

		outline_object->setEnabled(is_hovering);

		if (drag_start && is_target) {
			grab(mouse_position);
		} else if (drag_pressed && drag_data.is_dragging) {
			drag(mouse_position, delta_time);
		} else if (drag_end) {
			drop(mouse_position);
		}
	}

	// Object sawy on move
	Vector2 move_delta = object.getPosition() - last_position;
	Vector2 true_size = collider.getSize() / object.getScale();
	float sway_speed = 12.f - (true_size.magnitude() / 100.f);
	float rotation = utils::clamp(move_delta.x * 9.f, -45.f, 45.f);
	rotation = utils::lerp(object.getRotation(), rotation, delta_time * sway_speed);

	object.setRotation(rotation);
	last_position = object.getPosition();
}

void Drag::createOutline() {
	outline_object = new Object(game, "outline", &object);
	outline_object->setLocalPosition(Vector2(0.f, 0.f));
	new OutlineRenderer(game, *outline_object, collider.getSize());
	outline_object->setEnabled(false);
}

void Drag::grab(Vector2& mouse_position) {
	drag_data.is_dragging = true;
	drag_data.grab_offset = object.getPosition() - mouse_position;
	last_position = object.getPosition();

	object.setParent(nullptr);
	float size = collider.getSize().magnitude();
	float lift_scaling = 1.0f + (lift_size / size);
	object.setScale(Vector2::scale(lift_scaling));
	object.setZIndex(4);

	// Update dragging behaviour
	updateDroppableRegions();
	Collider* new_region = collider.getMostOverlapping(drag_data.droppable_regions);
	if (new_region != nullptr) drag_data.current_region = new_region;
	updateRegionLock();
}

void Drag::drag(Vector2& mouse_position, float delta_time) {
	Vector2 target_position = mouse_position + drag_data.grab_offset;
	if (drag_data.is_region_locked) {
		object.setPosition(target_position);
		collider.fitInto(drag_data.current_region);
	} else {
		// Clamp to the screen size
		target_position = Vector2::clamp(target_position, drag_bounds.min, drag_bounds.max);
		object.setPosition(target_position);
	}
}

void Drag::drop(Vector2& mouse_position) {
	drag_data.is_dragging = false;
	object.setScale(Vector2::scale(1.f));
	confineToRegion();
}

void Drag::confineToRegion() {
	if (drag_data.is_region_locked) {
		object.setParent(&drag_data.current_region->getObject()); // Set region as parent
		return;
	}
	Collider* fit_to = collider.getMostOverlapping(drag_data.droppable_regions);
	drag_data.current_region = fit_to == nullptr ? drag_data.current_region : fit_to;
	move_to(collider.getFitPosition(drag_data.current_region));
	object.setParent(&drag_data.current_region->getObject()); // Set region as parent
}
