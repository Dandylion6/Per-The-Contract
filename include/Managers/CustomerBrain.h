#pragma once

#include <stdint.h>

#include "Components/Customer.h"
#include "Data/DealData.h"

class CustomerBrain
{
public:
	// Functions
	static uint16_t generatePriceOffer(DealData& deal);
	static void determinePerceivedPrice(DealData& deal);
	static void playerOfferPenalty(DealData& deal);

	static bool isAcceptablePrice(DealData& deal);

	static bool willAcceptDeal(DealData& deal);
	static bool willNegotiate(DealData& deal);
	static bool willDeclineDeal(DealData& deal);

private:
	// Functions
	static uint16_t finilisePriceChange(DealData& deal, uint16_t change);
	static uint16_t calculateMinPrice(DealData& deal);
	static uint16_t calculateMaxPrice(DealData& deal);
	static float determineBasePriceChange(DealData& deal);
};