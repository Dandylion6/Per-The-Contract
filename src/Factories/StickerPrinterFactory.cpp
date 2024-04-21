#include <cstdint>
#include <string>

#include "Components/Collider.h"
#include "Components/Renderers/SpriteRenderer.h"
#include "Components/Renderers/TextRenderer.h"
#include "Components/Sticker/PrinterKey.h"
#include "Components/Sticker/StickerPrinter.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Factories/StickerPrinterFactory.h"
#include "Core/Utility/Vector2.h"


//_______________
// Constructors

StickerPrinterFactory::StickerPrinterFactory(Game& game) : game(game) {
	createPrinter();
	for (uint8_t i = 0u; i < 11u; i++) {
		createKey(i);
	}
}

StickerPrinterFactory::~StickerPrinterFactory() {
}


//____________________
// Private functions

void StickerPrinterFactory::createPrinter() {
	// Create printer object
	Object* printer_object = new Object(
		game, "sticker_printer", game.getObject("counter")
	);
	printer_object->setPosition(Vector2(770.f, 830.f));

	// Create printer display
	Object* display_object = new Object(
		game, "printer_display", printer_object
	);
	display_object->setLocalPosition(Vector2(50.f, 32.f));
	TextRenderer* display_renderer = new TextRenderer(
		game, *display_object, "000"
	);

	// Add components
	new SpriteRenderer(
		game, *printer_object, directory + "sticker_printer.png"
	);
	printer = new StickerPrinter(game, *printer_object, *display_renderer);
}

void StickerPrinterFactory::createKey(uint8_t index) const {
	// Create key object
	std::string name = "printer_key_" + std::to_string(index);
	Object* key_object = new Object(game, name, &printer->getObject());
	key_object->setLocalPosition(key_positions[index]);
	key_object->setZIndex(1);

	// Add components
	SpriteRenderer* renderer = new SpriteRenderer(
		game, *key_object, directory + name + ".png"
	);
	Collider* collider = new Collider(
		game, *key_object, renderer->getSize()
	);
	new PrinterKey(
		game, *key_object, *printer, *collider, index
	);
}
