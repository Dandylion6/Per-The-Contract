#pragma once

#include <cstdint>
#include <string>

#include "Components/Sticker/Sticker.h"
#include "Core/Managers/Game.h"

class StickerFactory
{
public:
	// Constructors
	StickerFactory(Game& game);
	virtual ~StickerFactory();

	// Getters
	static StickerFactory& getInstance();

	// Functions
	Sticker* createSticker(uint16_t current_price) const;

private:
	// Constants
	const std::string sticker_path = "assets/sprites/objects/sticker.png";

	// References
	Game& game;

	// Variables
	static StickerFactory* instance;
};

