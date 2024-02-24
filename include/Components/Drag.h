#pragma once

#include "Components/Renderers/Renderer.h"
#include "Core/Component.h"
#include "Core/Utility/Bounds.h"
#include "Core/Utility/Vector2.h"

// Forward declerations
class Collider;
class Game;
class Object;

class Drag : public Component
{
public:
	// Constructors
	Drag(
		Game& game, 
		Object& object,
		Renderer& renderer, 
		Collider& collider
	);
	virtual ~Drag();

	// Functions
	virtual void update(float delta_time) override;

protected:
	// References
	Renderer& renderer;
	Collider& collider;

	// Variables
	bool is_dragging = false;
	bool drag_pressed = false;
	Vector2 grab_offset = Vector2();
	Bounds drag_bounds = Bounds();
	Collider* last_dropped = nullptr;

	// Functions
	virtual void grab(Vector2& mouse_position);
	virtual void drag(Vector2& mouse_position, float delta_time);
	virtual void drop(Vector2& mouse_position);
};

