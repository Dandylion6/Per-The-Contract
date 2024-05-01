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
		Vector2(-46.5f, 62.f), Vector2(-82.f, 29.f), Vector2(-46.5f, 29.f), // 0, 1, 2
		Vector2(-10.5f, 29.f), Vector2(-82.f, -4.5f), Vector2(-46.5f, -4.5f), // 3, 4, 5
		Vector2(-10.5f, -4.5f), Vector2(-82.f, -37.f), Vector2(-46.5f, -37.f), // 6, 7, 8
		Vector2(-10.5f, -37.f), Vector2(-10.5f, 65.f), Vector2(-82.f, 65.f) // 9, 10, 11
	};

	// References
	Game& game;

	// Variables
	StickerPrinter* printer = nullptr;

	// Functions
	void createPrinter();
	void createKey(uint8_t index) const;
};