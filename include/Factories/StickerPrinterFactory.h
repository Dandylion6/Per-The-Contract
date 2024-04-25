#pragma once

#include <cstdint>
#include <string>

#include "Components/Sticker/StickerPrinter.h"
#include "Core/Managers/Game.h"
#include "Core/Utility/Vector2.h"

class StickerPrinterFactory
{
public:
	// Constructors
	StickerPrinterFactory(Game& game);
	virtual ~StickerPrinterFactory();

private:
	// Sprite paths
	const std::string directory = "assets/sprites/objects/sticker_printer/";
	const std::string printer_path = directory + "sticker_printer.png";

	// Other constants
	const Vector2 key_positions[12u] = {
		Vector2(-46.5f, 62.f), Vector2(-82.f, -36.f), Vector2(-46.5f, -36.f),
		Vector2(-10.5f, -36.f), Vector2(-82.f, -2.f), Vector2(-46.5f, -2.f),
		Vector2(-10.5f, -2.f), Vector2(-82.f, 29.f), Vector2(-46.5f, 29.f),
		Vector2(-10.5f, 29.f), Vector2(-10.5f, 65.f), Vector2(-82.f, 65.f)
	};

	// References
	Game& game;

	// Variables
	StickerPrinter* printer = nullptr;

	// Functions
	void createPrinter();
	void createKey(uint8_t index) const;
};