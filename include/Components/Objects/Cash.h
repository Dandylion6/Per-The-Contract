#pragma once

#include <stdint.h>

#include "Components/Collider.h"
#include "Components/Drag.h"
#include "Core/Component.h"
#include "Core/Utility/Vector2.h"

// Forward declerations
class Game;
class Object;

class Cash : public Drag
{
public:
	// Constructors
	Cash(
		Game& game,
		Object& object,
		Collider& collider,
		uint8_t value
	);
	virtual ~Cash();

	// Getters
	uint8_t getValue() const;

private:
	// Variables
	uint8_t value = 0u;

	// Functions
	void drop(Vector2& mouse_position) override;
	void updateRegionLock() override;
	void updateDroppableRegions() override;
};

