#include <cmath>
#include <stdint.h>

#include <SFML/System/Vector2.hpp>

#include "Core/Utility/Vector2.h"
#include "Core/Utility/Math.h"


//_______________
// Constructors

Vector2::Vector2() {
}

Vector2::Vector2(float x, float y) {
	this->x = x;
	this->y = y;
}

Vector2::Vector2(int x, int y) {
	this->x = static_cast<float>(x);
	this->y = static_cast<float>(y);
}

Vector2::Vector2(uint32_t x, uint32_t y) {
	this->x = static_cast<float>(x);
	this->y = static_cast<float>(y);
}

Vector2::Vector2(sf::Vector2f vector) {
	this->x = vector.x;
	this->y = vector.y;
}

Vector2::Vector2(sf::Vector2u vector) {
	this->x = static_cast<float>(vector.x);
	this->y = static_cast<float>(vector.y);
}

Vector2::Vector2(sf::Vector2i vector) {
	this->x = static_cast<float>(vector.x);
	this->y = static_cast<float>(vector.y);
}


//___________________
// Public functions

float Vector2::magnitude() {
	return sqrt(pow(this->x, 2.f) + pow(this->y, 2.f));
}

Vector2 Vector2::scale(float factor) {
	return Vector2(1.f, 1.f) * factor;
}

Vector2 Vector2::clamp(Vector2 vector, float min, float max) {
	float x = utils::clamp(vector.x, min, max);
	float y = utils::clamp(vector.y, min, max);
	return Vector2(x, y);
}

Vector2 Vector2::clamp(Vector2 vector, Vector2 min, Vector2 max) {
	float x = utils::clamp(vector.x, min.x, max.x);
	float y = utils::clamp(vector.y, min.y, max.y);
	return Vector2(x, y);
}

Vector2 Vector2::lerp(Vector2 start, Vector2 end, float time) {
	float x = utils::lerp(start.x, end.x, time);
	float y = utils::lerp(start.y, end.y, time);
	return Vector2(x, y);
}

Vector2 Vector2::outExpo(Vector2 start, Vector2 end, float time) {
	float x = utils::outExpo(start.x, end.x, time);
	float y = utils::outExpo(start.y, end.y, time);
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

bool Vector2::operator==(const Vector2& other) {
	return this->x == other.x && this->y == other.y;
}
