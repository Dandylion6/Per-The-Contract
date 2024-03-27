#pragma once

#include <cstdint>

#include "Components/Collider.h"
#include "Components/Drag.h"
#include "Components/Item.h"
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
		Collider& collider, uint16_t price
	);
	virtual ~Sticker();

private:
	// References
	DialogueManager& dialogue_manager;

	// Variables
	uint16_t price = 0u;

	// Functions
	bool assignToItem();
	void grab(Vector2& mouse_position) override;
	void drag(Vector2& mouse_position, float delta_time) override;
	void drop(Vector2& mouse_position) override;
	void handleDialogue(Item* item);
};