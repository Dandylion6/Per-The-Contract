#include <string>

#include "Components/Renderers/SpriteRenderer.h"
#include "Components/Sticker/StickerPrinter.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Factories/StickerPrinterFactory.h"


//_______________
// Constructors

StickerPrinterFactory::StickerPrinterFactory(Game& game) : game(game) {
	// Instatiate printer
	Object* printer_object = new Object(
		game, "sticker_printer", game.getObject("counter")
	);
	printer_object->setZIndex(1);
	printer_object->setPosition(Vector2(770.f, 830.f));

	SpriteRenderer* renderer = new SpriteRenderer(
		game, *printer_object, directory + "sticker_printer.png"
	);
	printer = new StickerPrinter(game, *printer_object);

	// Instantiate printer keys
	for (uint8_t i = 0u; i < 11u; i++) {
		createKey(i, printer_object);
	}
}

StickerPrinterFactory::~StickerPrinterFactory() {
}


//____________________
// Private functions

void StickerPrinterFactory::createKey(
	uint8_t index, Object* parent_object
) const {
	std::string name = "printer_key_" + std::to_string(index);
	Object* key_object = new Object(game, name, parent_object);
	key_object->setZIndex(2);
	key_object->setLocalPosition(key_positions[index]);

	SpriteRenderer* renderer = new SpriteRenderer(
		game, *key_object, directory + name + ".png"
	);
}
