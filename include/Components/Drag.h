#pragma once

#include "Components/Collider.h"
#include "Core/Component.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Core/Utility/Bounds.h"
#include "Core/Utility/Vector2.h"

class Drag : public Component
{
public:
	// Constructors
	Drag(Game& game, Object& object, Collider& collider);
	virtual ~Drag();

	// Functions
	virtual void update(float delta_time) override;

private:
	// References
	Collider& collider;

	// Variables
	bool is_dragging = false;
	bool drag_pressed = false;
	Vector2 grab_offset = Vector2();
	Bounds drag_bounds = Bounds();

	// Functions
	virtual void grab(Vector2& mouse_position);
	virtual void drag(Vector2& mouse_position);
	virtual void drop(Vector2& mouse_position);
};

