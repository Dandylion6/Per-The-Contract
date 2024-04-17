#pragma once

#include <cstdint>

#include "Data/DealData.h"

class CustomerBrain
{
public:
	// Functions
	static uint16_t generatePriceOffer(DealData& deal);
	static void determinePerceivedPrice(DealData& deal);

	static bool willAcceptDeal(DealData& deal);
	static bool willNegotiate(DealData& deal);
	static bool willDeclineDeal(DealData& deal);

private:
};