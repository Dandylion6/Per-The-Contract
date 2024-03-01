#pragma once

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
	void update(float delta_time) override;
};