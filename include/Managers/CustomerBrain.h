#pragma once

#include <stdint.h>

#include "Components/Customer.h"
#include "Data/DealData.h"

static class CustomerBrain
{
public:
	// Functions
	static uint16_t generatePriceOffer(DealData& deal);

	static bool isAcceptablePrice(DealData& deal);

	static bool willAcceptDeal(DealData& deal);
	static bool willNegotiate(DealData& deal);
	static bool willDeclineDeal(DealData& deal);

private:
	// Functions
	static float determineBasePriceChange(DealData& deal);
	static void playerOfferPenalty(DealData& deal);
};