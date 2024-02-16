#include <cmath>

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

Vector2::Vector2(sf::Vector2i vector) {
	this->x = vector.x;
	this->y = vector.y;
}


//___________________
// Public functions

float Vector2::magnitude() {
	return sqrt(pow(this->x, 2.f) + pow(this->y, 2.f));
}

Vector2 Vector2::clamp(Vector2 vector, float min, float max) {
	float x = vector.x < min ? min : (vector.x > max ? max : vector.x);
	float y = vector.y < min ? min : (vector.y > max ? max : vector.y);
	return Vector2(x, y);
}

Vector2 Vector2::clamp(Vector2 vector, Vector2 min, Vector2 max) {
	float x = vector.x < min.x ? min.x : (vector.x > max.x ? max.x : vector.x);
	float y = vector.y < min.y ? min.y : (vector.y > max.y ? max.y : vector.y);
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

void Vector2::operator+=(const Vector2& other) {
	this->x += other.x;
	this->y += other.y;
}
