#include <SFML/Window/Mouse.hpp>

#include "Components/Collider.h"
#include "Components/Drag.h"
#include"Components/Renderers/Renderer.h"
#include "Core/Component.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"

//_______________
// Constructors

Drag::Drag(
	Game& game,
	Object& object,
	Renderer& renderer,
	Collider& collider
) : 
	Component(game, object), 
	renderer(renderer), 
	collider(collider) 
{
	// Create drag limit bounds
	Vector2 extent = collider.getSize() * 0.5f;
	Vector2 window_size = game.getWindow().getSize();
	drag_bounds = Bounds(extent, window_size - extent);
	last_dropped = nullptr;
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
	bool drag_end = state_changed && !drag_pressed && is_dragging;

	if (drag_start) {
		grab(mouse_position);
	} else if (drag_pressed && is_dragging) {
		drag(mouse_position, delta_time);
	} else if (drag_end) {
		drop(mouse_position);
	}
}

void Drag::grab(Vector2& mouse_position) {
	is_dragging = true;
	grab_offset = object.getPosition() - mouse_position;
	object.setScale(Vector2::scale(1.1f));
	renderer.pushToFront();
}

void Drag::drag(Vector2& mouse_position, float delta_time) {
	Vector2 target_position = grab_offset + mouse_position;
	target_position = Vector2::clamp(
		target_position, drag_bounds.min, drag_bounds.max
	);
	object.setPosition(target_position);
}

void Drag::drop(Vector2& mouse_position) {
	is_dragging = false;
	object.setScale(Vector2::scale(1.f));
}
