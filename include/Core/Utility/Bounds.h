#pragma once

#include "Core/Utility/Vector2.h"

struct Bounds
{
	// Variables
	Vector2 min = Vector2();
	Vector2 max = Vector2();

	// Constructors
	Bounds();
	Bounds(Vector2 min, Vector2 max);
	Bounds(Vector2 position, Vector2 size, Vector2 anchor);

	// Functions
	bool overlaps(Bounds other) const;
	bool overlapsPoint(Vector2 point) const;
};