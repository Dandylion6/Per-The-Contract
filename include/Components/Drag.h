#pragma once

#include <vector>
#include <memory>

#include "Components/Renderers/Renderer.h"
#include "Core/Component.h"
#include "Core/Utility/Bounds.h"
#include "Core/Utility/Vector2.h"
#include "Data/DragData.h"
#include "Components/Collider.h"

// Forward declerations
class Game;
class Object;

class Drag : public Component
{
public:
	// Constructors
	Drag(Game& game, Object& object, Collider& collider);
	virtual ~Drag();

	// Functions
	void move_to(Vector2 position);
	virtual void update(float delta_time) override;

protected:
	// Constants
	const float move_time = 0.55f;

	// References
	Collider& collider;

	// Variables
	bool drag_pressed = false;
	Bounds drag_bounds = Bounds();

	std::unique_ptr<Vector2> move_position = nullptr;
	std::unique_ptr<Vector2> begin_position = nullptr;
	float time_moved = 0.f;

	Collider* storage_region = nullptr;
	Collider* send_region = nullptr;
	Collider* receive_region = nullptr;
	DragData drag_data = DragData();

	// Functions
	virtual void updateDroppableRegions() = 0;
	virtual void updateRegionLock() = 0;
	virtual void grab(Vector2& mouse_position);
	virtual void drag(Vector2& mouse_position, float delta_time);
	virtual void drop(Vector2& mouse_position);
	void confineToRegion();
};

