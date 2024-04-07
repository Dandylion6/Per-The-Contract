#pragma once

#include <cstdint>
#include <string>

#include "Components/Sticker/Sticker.h"

// Forward declerations
class Game;

class StickerFactory
{
public:
	// Constructors
	StickerFactory(Game& game);
	virtual ~StickerFactory();

	// Functions
	Sticker* createSticker(uint16_t current_price) const;

private:
	// Constants
	const std::string long_path = "assets/sprites/objects/stickers/long_sticker.png";
	const std::string middle_path = "assets/sprites/objects/stickers/sticker.png";
	const std::string short_path = "assets/sprites/objects/stickers/short_sticker.png";

	// References
	Game& game;
};

