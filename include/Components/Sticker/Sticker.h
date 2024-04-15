#pragma once

#include <cstdint>

#include "Components/Collider.h"
#include "Components/Drag.h"
#include "Components/Objects/Item.h"
#include "Managers/DialogueManager.h"

// Forward declerations
class Game;
class Object;

class Sticker : public Drag
{
public:
	// Constructors
	Sticker(
		Game& game, Object& object,
		Collider& collider, uint16_t current_price
	);
	virtual ~Sticker();

private:
	// Variables
	uint16_t current_price = 0u;

	// Functions
	bool assignToItem();
	void updateDroppableRegions() override;
	void updateRegionLock() override;
	void drop(Vector2& mouse_position) override;
	void handleDialogue(Item* item);
};