#pragma once

#include <cstdint>

#include "Core/Component.h"

// Forward declerations
class Game;
class Object;

class Sticker : public Component
{
public:
	// Constructors
	Sticker(Game& game, Object& object, uint16_t price);
	virtual ~Sticker();

	// Functions
	void update(float delta_time) override;

private:
	// Variables
	uint16_t price = 0u;
};