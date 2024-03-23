#pragma once

#include <memory>

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
		Collider& collider,
		std::weak_ptr<ItemData> data
	);
	virtual ~Item();

	// Getters
	uint16_t getPrice() const;

	// Setters
	void setOwned(bool owned_by_player);
	void setPrice(uint16_t price);

	// Functions
	void drag(Vector2& mouse_position, float delta_time) override;
	void drop(Vector2& mouse_position) override;

private:
	// Variables
	bool owned_by_player = false;
	std::weak_ptr<ItemData> data;
	uint16_t price = 0u;
	Collider* receive_region;
};