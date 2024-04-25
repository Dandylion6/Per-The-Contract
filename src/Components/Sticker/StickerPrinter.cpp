#include <cstdint>
#include <string>

#include "Components/Renderers/TextRenderer.h"

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
	text_display(text_display)
{
}

StickerPrinter::~StickerPrinter() {
}


//____________________
// Private functions

void StickerPrinter::keyOutput(uint8_t output) {
	// Start printing when print is pressed
	if (output == 10u) {
		sticker_price = ((sticker_price + 5u) / 10u) * 10u; // Round last number
		if (sticker_price > 0u) printSticker();
	}

	if (output == 10u || output == 11u) { // Delete all numbers
		// Reset output
		sticker_price = 0u;
		text_display.setText("0000");
		return;
	}

	uint16_t remainder = sticker_price % 1000u; // Push 3rd digit if above 100
	sticker_price = (remainder * 10u) + output;

	// Set display text
	std::string print_display = std::to_string(sticker_price);
	while (print_display.length() < 4) { // Pad with 0s
		print_display = "0" + print_display;
	}
	text_display.setText(print_display);
}

void StickerPrinter::update(float delta_time) {
}

void StickerPrinter::printSticker() {
	Sticker* sticker = StickerFactory::getInstance().createSticker(sticker_price);
	sticker->getObject().setPosition(object.getPosition() + Vector2(-47.f, -50.f));
	sticker->getObject().setRotation(90.f);

	float x = utils::Random::generateFloat(-drop_range, drop_range);
	float y = utils::Random::generateFloat(-drop_range, drop_range);
	Vector2 position = sticker->getObject().getPosition() + Vector2(x, y - 130.f);
	sticker->move_to(position);
}
