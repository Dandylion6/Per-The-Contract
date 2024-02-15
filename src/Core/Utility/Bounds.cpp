#include "Core/Utility/Bounds.h"
#include "Core/Utility/Vector2.h"


//_______________
// Constructors

Bounds::Bounds() {
}

Bounds::Bounds(Vector2 min, Vector2 max) {
	this->min;
	this->max;
}

Bounds::Bounds(Vector2 position, Vector2 size, Vector2 anchor) {
	anchor = Vector2::clamp(anchor, 0.f, 1.f);
	this->min = position - (size * 0.5f * anchor);
	this->max = this->min + (size * 0.5f);
}


//___________________
// Public functions

bool Bounds::overlaps(Bounds other) const {
	bool overlapX = (this->min.x < other.max.x) && (this->max.x > other.min.x);
	bool overlapY = (this->min.y < other.max.y) && (this->max.y > other.min.y);
	return overlapX && overlapY;
}

bool Bounds::overlapsPoint(Vector2 point) const {
	bool overlapX = (point.x >= this->min.x) && (point.x <= this->max.x);
	bool overlapY = (point.y >= this->min.y) && (point.y <= this->max.y);
	return overlapX && overlapY;
}
