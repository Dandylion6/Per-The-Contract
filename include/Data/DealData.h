#pragma once

#include <stdint.h>
#include <unordered_map>

#include "Components/Customer.h"
#include "Components/Item.h"
#include "Core/Managers/Game.h"
#include "Data/CharacterData.h"

struct DealData
{
	// Constants
	const std::unordered_map<CustomerTrait, float> negotiability_trait{
		{ CustomerTrait::Assertive, 0.2f },
		{ CustomerTrait::OpenMinded, 0.6f },
		{ CustomerTrait::Knowledgeable, 0.5f },
		{ CustomerTrait::Frugal,  0.8f },
		{ CustomerTrait::Impulsive, 0.3f },
		{ CustomerTrait::Trusting, 0.4f }
	};
	const std::unordered_map<CustomerTrait, float> acceptable_range_trait{
		{ CustomerTrait::Assertive, 0.2f },
		{ CustomerTrait::OpenMinded, 0.4f },
		{ CustomerTrait::Knowledgeable, 0.3f },
		{ CustomerTrait::Frugal, 0.25f },
		{ CustomerTrait::Impulsive, 0.6f },
		{ CustomerTrait::Trusting, 0.5f }
	};

	// Constructor
	DealData(
		CustomerTrait trait,
		CustomerRequest request,
		uint16_t funds,
		Item* item,
		float base_willingness
	) : request(request), funds(funds)
	{
		this->item = item;
		this->negotiability_factor = negotiability_trait.find(trait)->second;
		this->acceptable_range_factor = acceptable_range_trait.find(trait)->second;
		this->willingness_factor = base_willingness;
	}

	// Variables
	const CustomerRequest request;
	const uint16_t funds = 0u;

	Item* item = nullptr;
	uint16_t acceptable_price = 0u;
	uint16_t perceived_item_value = 0u;
	float negotiability_factor = 0.f;
	float acceptable_range_factor = 0.f;
	float willingness_factor = 0.f;
};