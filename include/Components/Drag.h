#pragma once

#include <memory>

#include "Components/Collider.h"
#include "Core/Component.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Core/Utility/Bounds.h"
#include "Core/Utility/Vector2.h"
#include "Data/DragData.h"

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
	const float move_time = 0.7f;
	const float lift_size = 20.f;

	// References
	Collider& collider;

	// Variables
	bool drag_pressed = false;
	bool is_hovering = false;
	Bounds drag_bounds = Bounds();

	std::unique_ptr<Vector2> move_position = nullptr;
	std::unique_ptr<Vector2> begin_position = nullptr;
	float time_moved = 0.f;

	Collider* storage_region = nullptr;
	Collider* send_region = nullptr;
	Collider* receive_region = nullptr;
	Object* outline_object = nullptr;
	DragData drag_data = DragData();
	Vector2 last_position = Vector2();

	// Functions
	void createOutline();
	virtual void updateDroppableRegions() = 0;
	virtual void updateRegionLock() = 0;
	virtual void grab(Vector2& mouse_position);
	virtual void drag(Vector2& mouse_position, float delta_time);
	virtual void drop(Vector2& mouse_position);
	void confineToRegion();
};

