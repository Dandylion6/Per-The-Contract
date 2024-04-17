#include <cstdint>

#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/Mouse.hpp"

#include "Components/Collider.h"
#include "Components/Sticker/PrinterKey.h"
#include "Components/Sticker/StickerPrinter.h"
#include "Core/Component.h"
#include "Core/Object.h"
#include "Core/Utility/Vector2.h"


//_______________
// Constructors

PrinterKey::PrinterKey(
	Game& game, 
	Object& object, 
	StickerPrinter& printer, 
	Collider& collider,
	uint8_t output
) : 
	Component(game, object), 
	printer(printer), 
	collider(collider),
	output(output) {
	key = key_map.find(output)->second;
	num_key = key_map.find(output + 11u)->second;
}

PrinterKey::~PrinterKey() {
}


//___________________
// Public functions

void PrinterKey::update(float delta_time) {
	handleKeyInput();
	handleMouseInput();
}


//____________________
// Private functions

void PrinterKey::handleKeyInput() {
	bool is_inputing = sf::Keyboard::isKeyPressed(key);
	is_inputing = is_inputing || sf::Keyboard::isKeyPressed(num_key);

	bool is_releasable = key_pressed && !target_clicked;
	bool is_pressable = !key_pressed && !target_clicked;

	if (!is_inputing && is_releasable) keyRelease();
	if (is_inputing && is_pressable) keyPress();
	key_pressed = is_inputing;
}

void PrinterKey::handleMouseInput() {
	Vector2 mouse_position = sf::Mouse::getPosition();
	bool is_inputing = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	bool is_target = collider.pointHits(mouse_position);

	bool is_releasable = target_clicked && !key_pressed;
	bool is_pressable = !mouse_clicked && !key_pressed;

	if ((!is_inputing || !is_target) && is_releasable) {
		target_clicked = false;
		keyRelease();
	}

	if (is_inputing && is_target && is_pressable) {
		target_clicked = true;
		keyPress();
	}
	mouse_clicked = is_inputing;
}

void PrinterKey::keyPress() {
	object.setScale(Vector2::scale(0.9f));
	printer.keyOutput(output);
}

void PrinterKey::keyRelease() {
	object.setScale(Vector2::scale(1.f));
}