#pragma once

#include <cstdint>

#include "SFML/System/Vector2.hpp"

struct Vector2
{
	float x = 0;
	float y = 0;

	// Constructors
	Vector2();
	Vector2(float x, float y);
	Vector2(int x, int y);
	Vector2(uint32_t x, uint32_t y);
	Vector2(sf::Vector2f vector);
	Vector2(sf::Vector2u vector);
	Vector2(sf::Vector2i vector);

	// Conversions
	operator sf::Vector2f() const { return sf::Vector2f(x,y ); }

	// Functions
	float magnitude();

	static Vector2 scale(float factor);
	static Vector2 clamp(Vector2 vector, float min, float max);
	static Vector2 clamp(Vector2 vector, Vector2 min, Vector2 max);
	static Vector2 lerp(Vector2 start, Vector2 end, float time);
	static Vector2 outExpo(Vector2 start, Vector2 end, float time);

	// Operands
	Vector2 operator+(const Vector2& other) const;
	Vector2 operator-(const Vector2& other) const;
	Vector2 operator*(const Vector2& other) const;
	Vector2 operator/(const Vector2& other) const;
	Vector2 operator*(const float factor) const;
	Vector2 operator/(const float factor) const;

	void operator+=(const Vector2& other);
	bool operator==(const Vector2& other);
};