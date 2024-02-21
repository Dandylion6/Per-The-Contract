#include <algorithm>

#include "Core/Utility/Bounds.h"
#include "Core/Utility/Vector2.h"


//_______________
// Constructors

Bounds::Bounds() {
}

Bounds::Bounds(Vector2 min, Vector2 max) {
	this->min = min;
	this->max = max;
	this->center = (max + min) / 2.f;
}

Bounds::Bounds(Vector2 position, Vector2 size, Vector2 anchor) {
	anchor = Vector2::clamp(anchor, 0.f, 1.f);
	this->min = position - size * anchor;
	this->max = this->min + size;
	this->center = (max + min) / 2.f;
}


//___________________
// Public functions

bool Bounds::overlaps(const Bounds& other) const {
	bool overlapX = (this->min.x < other.max.x) && (this->max.x > other.min.x);
	bool overlapY = (this->min.y < other.max.y) && (this->max.y > other.min.y);
	return overlapX && overlapY;
}

bool Bounds::overlapsPoint(const Vector2& point) const {
	bool overlapX = (point.x >= this->min.x) && (point.x <= this->max.x);
	bool overlapY = (point.y >= this->min.y) && (point.y <= this->max.y);
	return overlapX && overlapY;
}

Vector2 Bounds::getClosestPoint(const Vector2& point) const {
	return Vector2::clamp(point, min, max);
}

float Bounds::getDistanceTo(const Bounds& other) const {
	Vector2 point1 = this->getClosestPoint(other.center);
	Vector2 point2 = other.getClosestPoint(this->center);
	return (point2 - point1).magnitude();
}

float Bounds::getOverlapArea(const Bounds& other) const {
	Vector2 min_overlap = Vector2(
		std::min(max.x, other.max.x), std::min(max.y, other.max.y)
	);
	Vector2 max_overlap = Vector2(
		std::max(min.x, other.min.x), std::max(min.y, other.min.y)
	);

	float x_overlap = std::max(0.f, min_overlap.x - max_overlap.x);
	float y_overlap = std::max(0.f, min_overlap.y - max_overlap.y);
	return x_overlap * y_overlap;
}
