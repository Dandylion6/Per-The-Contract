#include <SFML/Window/Mouse.hpp>

#include "Components/Buttons/Button.h"
#include "Components/Collider.h"
#include "Core/Component.h"
#include "Core/Managers/Game.h"


//_______________
// Constructors

Button::Button(
	Game& game, Object& object, Collider& collider
) :
	Component(game, object), collider(collider)
{
}

Button::~Button() {
}


//___________________
// Public functions

void Button::update(float delta_time) {
	bool mouse_press = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
	bool is_mouse_target = collider.pointHits(sf::Mouse::getPosition());
	bool input_started = mouse_pressed != mouse_press;

	if (input_started && mouse_press && is_mouse_target) buttonPressed();
	if (input_started && !mouse_press) buttonReleased();
	mouse_pressed = mouse_press;
}