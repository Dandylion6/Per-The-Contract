#include <SFML/Window/Mouse.hpp>

#include "Components/Drag.h"
#include "Core/Component.h"


//_______________
// Constructors

Drag::Drag(
	Game& game, Object& object, Collider& collider, Layer target_layer
) : Component(game, object), collider(collider) {
	this->target_layer = target_layer;
}

Drag::~Drag() {
}


//___________________
// Public functions

void Drag::update(float delta_time) {
	Vector2 mouse_position = sf::Mouse::getPosition();
	Bounds bounds = collider.getBounds();

	bool button_pressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	bool state_changed = button_pressed != drag_pressed;
	drag_pressed = button_pressed;

	bool mouse_in_bounds = bounds.overlapsPoint(mouse_position);
	bool drag_start = state_changed && drag_pressed && mouse_in_bounds;

	if (drag_start) {
		grab(mouse_position);
		return;
	}

	if (drag_pressed && is_dragging) {
		drag(mouse_position);
		return;
	}
	drop(mouse_position);
}

void Drag::grab(Vector2& mouse_position) {
	is_dragging = true;
	grab_offset = object.getPosition() - mouse_position;
}

void Drag::drag(Vector2& mouse_position) {
	object.setPosition(grab_offset + mouse_position);
}

void Drag::drop(Vector2& mouse_position) {
	is_dragging = false;
}
