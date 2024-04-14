#pragma once

#include <stdint.h>
#include <unordered_map>
#include <string>

#include "Data/CustomerTrait.h"
#include "Components/Item.h"
#include "Core/Managers/Game.h"
#include "Data/ItemData.h"

struct DealData
{
	// Constructor
	DealData(
		CustomerTrait trait,
		CustomerRequest request
	) : trait(trait), request(request)
	{
	}

	// Variables
	const CustomerTrait trait;
	const CustomerRequest request;

	Item* offered_item = nullptr;
	std::string request_id = std::string();
};