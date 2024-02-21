#pragma once

#include "Core/Utility/Vector2.h"

struct Bounds
{
	// Variables
	Vector2 min = Vector2();
	Vector2 max = Vector2();
	Vector2 center = Vector2();

	// Constructors
	Bounds();
	Bounds(Vector2 min, Vector2 max);
	Bounds(Vector2 position, Vector2 size, Vector2 anchor);

	// Functions
	bool overlaps(const Bounds& other) const;
	bool overlapsPoint(const Vector2& point) const;
	Vector2 getClosestPoint(const Vector2& point) const;
	float getDistanceTo(const Bounds& other) const;
	float getOverlapArea(const Bounds& other) const;
};