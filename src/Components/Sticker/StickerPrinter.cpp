#include "Components/Sticker/Sticker.h"
#include "Components/Sticker/StickerPrinter.h"
#include "Core/Component.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Core/Utility/Vector2.h"
#include "Factories/StickerFactory.h"


//_______________
// Constructors

StickerPrinter::StickerPrinter(
	Game& game, Object& object, TextRenderer& text_display
) : 
	Component(game, object), 
	text_display(text_display),
	sticker_factory(*game.getStickerFactory().lock()) {
}

StickerPrinter::~StickerPrinter() {
}


//____________________
// Private functions

void StickerPrinter::keyOutput(uint8_t output) {
	// Start printing when print is pressed
	if (output == 10u) {
		if (sticker_price > 0u) printSticker();

		// Reset output
		sticker_price = 0u;
		text_display.setText("000");
		return;
	}

	// Add new digit behind existing print value, removing the 3rd digit
	sticker_price = ((sticker_price % 100u) * 10u) + output;

	// Set display text
	std::string print_display = std::to_string(sticker_price);
	while (print_display.length() < 3) { // Pad with 0s
		print_display = "0" + print_display;
	}
	text_display.setText(print_display);
}

void StickerPrinter::update(float delta_time) {
}

void StickerPrinter::printSticker() {
	Sticker* sticker = sticker_factory.createSticker(sticker_price);
	Object& sticker_object = sticker->getObject();

	Vector2 position = object.getPosition() + Vector2(42.f, -130.f);
	sticker_object.setPosition(position);
}
