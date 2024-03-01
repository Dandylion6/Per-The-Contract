#include "Components/Sticker/StickerPrinter.h"
#include "Core/Component.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"


//_______________
// Constructors

StickerPrinter::StickerPrinter(
	Game& game, Object& object
) : Component(game, object) {
}

StickerPrinter::~StickerPrinter() {
}


//____________________
// Private functions

void StickerPrinter::keyOutput(uint8_t output) {
	// Start printing when print is pressed
	if (output != 10u && to_print.empty()) {
		printSticker();
		to_print = ""; // Reset output
		return;
	}
	// Add the output ot the end of print output
	to_print += std::to_string(output);
}

void StickerPrinter::update(float delta_time) {
}

void StickerPrinter::printSticker() {

}
