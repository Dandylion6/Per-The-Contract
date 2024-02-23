#pragma once

#include <string>

struct ItemData
{
	// Variables
	std::string item_id = "";
	std::string name = "Not found";
	std::string sprite_path = "";
	uint32_t market_value = 0u;

	// Constructors
	ItemData(
		std::string item_id,
		std::string name,
		std::string sprite_path,
		uint32_t market_value
	) {
		this->item_id = item_id;
		this->name = name;
		this->sprite_path = sprite_path;
		this->market_value = market_value;
	}
};