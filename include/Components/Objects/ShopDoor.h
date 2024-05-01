#pragma once

#include <string>

#include "Core/Component.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Core/Utility/Vector2.h"

class ShopDoor : public Component {
public:
	// Constructor
	ShopDoor(Game& game, Object& object);
	virtual ~ShopDoor();

	// Getters
	bool isMoving() const;

	// Functions
	void closeDoor();
	void openDoor();
	virtual void update(float delta_time) override;

private:
	// Constants
	const Vector2 closed_position = Vector2(0.f, 0.f);
	const Vector2 opened_position = Vector2(0.f, -764.f);
	const float extra_closed_wait = 0.75f;
	const float close_time = 1.25f;
	const float opening_time = 1.5f;

	// Variables
	float animation_time = 0.f;
	bool is_closing = false;
	bool is_opening = false;

	// Functions
	Vector2 animationFunction(float time);
};