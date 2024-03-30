#pragma once

#include <stdint.h>

#include "Components/Collider.h"
#include "Components/Drag.h"
#include "Core/Component.h"

// Forward declerations
class Game;
class Object;

class Cash : public Drag
{
	// Constructors
	Cash(
		Game& game,
		Object& object,
		Collider& collider,
		uint8_t value
	);
	virtual ~Cash();

private:
	// Variables
	uint8_t value = 0u;

	// Functions
	void updateRegionLock() override;
	void updateDroppableRegions() override;
};

