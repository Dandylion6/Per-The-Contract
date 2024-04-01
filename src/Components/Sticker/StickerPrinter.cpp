#include "Components/Sticker/Sticker.h"
#include "Components/Sticker/StickerPrinter.h"
#include "Core/Component.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Core/Utility/RandomGenerator.h"
#include "Core/Utility/Vector2.h"
#include "Factories/StickerFactory.h"


//_______________
// Constructors

StickerPrinter::StickerPrinter(
	Game& game, Object& object, TextRenderer& text_display
) : 
	Component(game, object), 
	text_display(text_display),
	sticker_factory(game.getStickerFactory()) {
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

	uint8_t remainder = sticker_price % 100u; // Push 3rd digit if above 100
	sticker_price = (remainder * 10u) + output;

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
	sticker_price = ((sticker_price + 5u) / 10u) * 10u; // Round last number
	Sticker* sticker = sticker_factory.createSticker(sticker_price);
	Object& sticker_object = sticker->getObject();

	int x = utils::Random::generateInt(-drop_range, drop_range);
	int y = utils::Random::generateInt(-drop_range, drop_range);
	Vector2 position = object.getPosition() + Vector2(42.f + x, 130.f + y);
	sticker_object.setPosition(position);
}
