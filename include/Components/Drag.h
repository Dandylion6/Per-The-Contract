#pragma once

#include "Components/Collider.h"
#include "Core/Component.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Core/Utility/Vector2.h"

class Drag : public Component
{
public:
	// Constructors
	Drag(
		Game& game,
		Object& object,
		Collider& collider,
		Layer drop_target
	);
	virtual ~Drag();

	// Functions
	void update(float delta_time) override;

private:
	// References
	Collider& collider;

	// Variables
	Layer target_layer = Layer::Default;
	bool is_dragging = false;
	bool drag_pressed = false;
	Vector2 grab_offset = Vector2();

	// Functions
	void grab(Vector2& mouse_position);
	void drag(Vector2& mouse_position);
	void drop(Vector2& mouse_position);
};

