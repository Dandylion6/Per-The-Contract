#pragma once

#include <stdint.h>
#include <unordered_map>

#include "Data/CustomerTrait.h"
#include "Components/Item.h"
#include "Core/Managers/Game.h"

struct DealData
{
	// Constructor
	DealData(
		CustomerTrait trait,
		CustomerRequest request,
		Item* item
	) : trait(trait), request(request)
	{
		this->item = item;
	}

	// Variables
	const CustomerTrait trait;
	const CustomerRequest request;
	const uint16_t funds = 0u;

	Item* item = nullptr;
	uint16_t acceptable_price = 0u;
	uint16_t perceived_item_value = 0u;
	float negotiability = 0.f;
	float acceptable_range = 0.f;
	float willingness = 0.f;
};