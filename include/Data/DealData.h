#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include "Components/Objects/Item.h"
#include "Core/Managers/Game.h"
#include "Data/CustomerTrait.h"

struct DealData
{
	// Constructor
	DealData(
		CustomerTrait customer_trait,
		CustomerRequest request
	) : customer_trait(customer_trait), request(request)
	{
	}

	// Variables
	const CustomerTrait customer_trait;
	const CustomerRequest request;

	Item* offered_item = nullptr;
	std::string request_id = std::string();

	std::unique_ptr<uint16_t> customer_accepted_price = nullptr;
	bool deal_started = false;
	bool deal_agreed = false;
};