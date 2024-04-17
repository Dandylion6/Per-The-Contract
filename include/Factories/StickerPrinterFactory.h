#pragma once

#include <cstdint>
#include <string>

#include "Components/Sticker/StickerPrinter.h"
#include "Core/Utility/Vector2.h"

// Forward declerations
class Game;
class Object;

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
	const Vector2 key_positions[11u] = {
		Vector2(-64.f, 48.f), Vector2(-64.f, 18.f), Vector2(-36.f, 18.f),
		Vector2(-8.f, 18.f), Vector2(-64.f, -14.f), Vector2(-36.f, -14.f),
		Vector2(-8.f, -14.f), Vector2(-64.f, -46.f), Vector2(-36.f, -46.f),
		Vector2(-8.f, -46.f), Vector2(-22.f, 48.f)
	};

	// References
	Game& game;

	// Variables
	StickerPrinter* printer = nullptr;

	// Functions
	void createPrinter();
	void createKey(uint8_t index) const;
};