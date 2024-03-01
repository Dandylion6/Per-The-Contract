#pragma once

#include <string>

#include "Core/Component.h"

// Forward declerations
class Game;
class Object;

class StickerPrinter : public Component
{
public:
	// Constructors
	StickerPrinter(Game& game, Object& object);
	virtual ~StickerPrinter();

	// Functions
	void keyOutput(uint8_t output);
	void update(float delta_time) override;

private:
	// Variables
	std::string to_print = "";

	// Functions
	void printSticker();
};