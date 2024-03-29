#pragma once

#include <string>

#include "Components/Renderers/TextRenderer.h"
#include "Core/Component.h"
#include "Factories/StickerFactory.h"

// Forward declerations
class Game;
class Object;

class StickerPrinter : public Component
{
public:
	// Constructors
	StickerPrinter(
		Game& game, Object& object, TextRenderer& text_display
	);
	virtual ~StickerPrinter();

	// Functions
	void keyOutput(uint8_t output);
	void update(float delta_time) override;

private:
	// Constants
	const uint8_t drop_range = 20u;

	// References
	TextRenderer& text_display;
	StickerFactory& sticker_factory;

	// Variables
	uint16_t sticker_price = 0u;

	// Functions
	void printSticker();
};