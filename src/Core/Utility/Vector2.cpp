#include <SFML/System/Vector2.hpp>

#include "Core/Utility/Vector2.h"


//_______________
// Constructors

Vector2::Vector2() {
}

Vector2::Vector2(float x, float y) {
	this->x = x;
	this->y = y;
}

Vector2::Vector2(sf::Vector2f vector) {
	this->x = vector.x;
	this->y = vector.y;
}

Vector2::Vector2(sf::Vector2u vector) {
	this->x = vector.x;
	this->y = vector.y;
}


//___________________
// Public functions

Vector2 Vector2::clamp(Vector2 vector, float min, float max) {
	float x = vector.x < min ? min : (vector.x > max ? max : vector.x);
	float y = vector.y < min ? min : (vector.y > max ? max : vector.y);
	return Vector2(x, y);
}


//____________
// Operators

Vector2 Vector2::operator+(const Vector2& other) const {
	return Vector2(this->x + other.x, this->y + other.y);
}

Vector2 Vector2::operator-(const Vector2& other) const {
	return Vector2(this->x - other.x, this->y - other.y);
}

Vector2 Vector2::operator*(const Vector2& other) const {
	return Vector2(this->x * other.x, this->y * other.y);
}

Vector2 Vector2::operator/(const Vector2& other) const {
	return Vector2(this->x / other.x, this->y / other.y);
}

Vector2 Vector2::operator*(const float factor) const {
	return Vector2(this->x * factor, this->y * factor);
}

Vector2 Vector2::operator/(const float factor) const {
	return Vector2(this->x / factor, this->y / factor);
}