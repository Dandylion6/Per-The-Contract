#pragma once

#include "Components/Collider.h"
#include "Components/Drag.h"
#include "Components/Renderers/Renderer.h"
#include "Core/Utility/Vector2.h"
#include "Data/ItemData.h"

// Forward declerations
class Game;
class Object;

class Item : public Drag
{
public:
	// Constructors
	Item(
		Game& game,
		Object& object,
		Renderer& renderer,
		Collider& collider,
		ItemData data
	);
	virtual ~Item();

	// Setters
	void setOwned(bool owned_by_player);

	// Functions
	void drag(Vector2& mouse_position, float delta_time) override;
	void drop(Vector2& mouse_position) override;

private:
	// Variables
	ItemData data;
	bool owned_by_player = false;
	Collider* receive_region;
};