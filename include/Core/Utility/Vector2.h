#pragma once

#include <SFML/System/Vector2.hpp>

struct Vector2
{
	float x = 0;
	float y = 0;

	// Constructors
	Vector2();
	Vector2(float x, float y);
	Vector2(sf::Vector2f vector);
	Vector2(sf::Vector2u vector);

	// Conversions
	operator sf::Vector2f() const { return sf::Vector2f(x,y ); }

	// Functions
	static Vector2 clamp(Vector2 vector, float min, float max);

	// Operands
	Vector2 operator+(const Vector2& other) const;
	Vector2 operator-(const Vector2& other) const;
	Vector2 operator*(const Vector2& other) const;
	Vector2 operator/(const Vector2& other) const;
	Vector2 operator*(const float factor) const;
	Vector2 operator/(const float factor) const;
};