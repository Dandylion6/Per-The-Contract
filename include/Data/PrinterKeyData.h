#pragma once

#include <string>

#include "Core/Utility/Vector2.h"

struct PrinterKeyData
{
	// Variables
	std::string sprite_path = "";
	uint8_t output = 0u;
	Vector2 position = Vector2();

	// Constructors
	PrinterKeyData(
		std::string sprite_path, uint8_t output, Vector2 position
	) {
		this->sprite_path = sprite_path;
		this->output = output;
		this->position = position;
	};
};