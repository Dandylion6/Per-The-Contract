#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

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
}

PrinterKey::~PrinterKey() {
}


//___________________
// Public functions

void PrinterKey::update(float delta_time) {
	if (!keyPressed()) {
		object.setScale(Vector2::scale(1.f));
		return;
	}
	object.setScale(Vector2::scale(0.9f));
	printer.keyOutput(output);
}

bool PrinterKey::keyPressed() const {
	Vector2 mouse_position = sf::Mouse::getPosition();
	if (!collider.pointHits(mouse_position, Layer::Default)) return false;
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) return false;
	return true;
}
