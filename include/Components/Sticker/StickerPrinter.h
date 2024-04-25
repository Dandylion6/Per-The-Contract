#pragma once

#include <cstdint>

#include "Components/Renderers/TextRenderer.h"
#include "Core/Component.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"

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
	const float drop_range = 20.f;

	// References
	TextRenderer& text_display;

	// Variables
	uint16_t sticker_price = 0u;

	// Functions
	void printSticker();
};