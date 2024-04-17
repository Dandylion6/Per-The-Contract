#pragma once

#include <vector>

#include "Components/Collider.h"
#include "Core/Utility/Vector2.h"

struct DragData
{
	// Constructors
	DragData() {
	};

	// Variables
	bool is_dragging = false;
	Vector2 grab_offset = Vector2();

	Collider* current_region = nullptr;
	std::vector<Collider*> droppable_regions;
	bool is_region_locked = false;
};