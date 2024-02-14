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
