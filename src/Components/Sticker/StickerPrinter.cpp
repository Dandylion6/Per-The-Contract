#include "Components/Sticker/StickerPrinter.h"
#include "Core/Component.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"


//_______________
// Constructors

StickerPrinter::StickerPrinter(
	Game& game, Object& object, TextRenderer& text_display
) : Component(game, object), text_display(text_display)  {
}

StickerPrinter::~StickerPrinter() {
}


//____________________
// Private functions

void StickerPrinter::keyOutput(uint8_t output) {
	// Start printing when print is pressed
	if (output == 10u && print_value > 0u) {
		printSticker();

		// Reset output
		print_value = 0u;
		text_display.setText("000");
		return;
	}
	
	// Can't have more than 3 digits
	if (print_value >= 100u) return;

	// Add new digit behind existing print value
	print_value = (print_value * 10u) + output;

	// Set display text
	std::string print_display = std::to_string(print_value);
	while (print_display.length() < 3) { // Pad with 0s
		print_display = "0" + print_display;
	}
	text_display.setText(print_display);
}

void StickerPrinter::update(float delta_time) {
}

void StickerPrinter::printSticker() {

}
