#pragma once

#include <stdint.h>
#include <string>

struct ItemData
{
	// Variables
	std::string item_id = "";
	std::string name = "Not found";
	std::string sprite_path = "";
	uint16_t market_value = 0u;
	uint8_t rarity;

	// Constructors
	ItemData(
		std::string item_id,
		std::string name,
		std::string sprite_path,
		uint16_t market_value,
		uint8_t rarity
	) {
		this->item_id = item_id;
		this->name = name;
		this->sprite_path = sprite_path;
		this->market_value = market_value;
		this->rarity = rarity;
	}
};