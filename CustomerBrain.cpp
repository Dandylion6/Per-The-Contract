#include <cmath>
#include <iostream>
#include <stdint.h>

#include "Components/Customer.h"
#include "Core/Managers/Game.h"
#include "Core/Utility/RandomGenerator.h"
#include "Data/DealData.h"
#include "Managers/CustomerBrain.h"


//___________________
// Public functions

uint16_t CustomerBrain::generatePriceOffer(DealData& deal) {
	uint16_t offer = deal.item->getCurrentPrice();
	float price_change_factor = determineBasePriceChange(deal);

	// Negotiators are willing to push the price more
	bool first_offer = deal.item->getLastPrice() == 0u;
	price_change_factor += first_offer ? (deal.negotiability_factor * 1.5f) + 0.1f : 0.f;
	price_change_factor += deal.negotiability_factor * 0.35f;

	// Random adjustment
	price_change_factor += utils::Random::generateFloat(-0.1f, 0.15f);

	uint16_t price_change = offer * price_change_factor;
	uint16_t new_offer = offer - price_change; // Can't exceed funds when buying
	new_offer = std::min(new_offer, deal.funds);
	if (deal.request == CustomerRequest::Selling) {
		new_offer = offer + price_change;
	}
	new_offer = ((new_offer + 5u) / 10u) * 10u; // Round last number
	return new_offer;
}

bool CustomerBrain::isAcceptablePrice(DealData& deal) {
	uint16_t offered_price = deal.item->getCurrentPrice();
	uint16_t range = deal.perceived_item_value * deal.acceptable_range_factor;
	if (deal.request == CustomerRequest::Selling) {
		uint16_t minimum = deal.perceived_item_value - range;
		return offered_price >= minimum;
	} else {
		uint16_t maximum = deal.perceived_item_value + range;
		return offered_price <= maximum;
	}
}

bool CustomerBrain::willAcceptDeal(DealData& deal) {
	// Certain customers might be more willing to accept or negotiate futher
	float accept_chance = 1.f - (deal.negotiability_factor * 0.7f);
	float accept_value = utils::Random::generateFloat(0.f, accept_chance);

	float acceptance_threshold = deal.negotiability_factor * 0.4f;
	if (deal.negotiability_factor < 0.3f) {
		acceptance_threshold -= 0.35f; // Low negotiability will more likely accept
	}

	// If willingness is low they are more likely to accept instead of continuing
	acceptance_threshold -= (deal.willingness_factor * 0.4f) - 0.35f;
	return accept_value > acceptance_threshold;
}

bool CustomerBrain::willNegotiate(DealData& deal) {
	// The chance at negotiating influenced by willingness
	float negotiate_chance = deal.willingness_factor * 0.8f;
	float negotiate_value = utils::Random::generateFloat(0.1f, negotiate_chance);

	// A high negiability is almost garanteed to negotiate
	float threshold = 1.f - deal.negotiability_factor;
	return negotiate_value > threshold;
}

bool CustomerBrain::willDeclineDeal(DealData& deal) {
	return deal.willingness_factor < 0.35f;
}


//____________________
// Private functions

float CustomerBrain::determineBasePriceChange(DealData& deal) {
	float offered_price = static_cast<float>(deal.item->getCurrentPrice());
	if (!isAcceptablePrice(deal)) {
		// Make the mimimum base offer
		uint16_t range = deal.perceived_item_value * deal.acceptable_range_factor;
		float difference = (deal.perceived_item_value + range) - offered_price;
		if (deal.request == CustomerRequest::Selling) {
			difference = (deal.perceived_item_value - range) - offered_price;
		}
		return difference / offered_price;
	}

	// Try making a compormise based on willingness
	float difference = deal.acceptable_range_factor - offered_price;
	float compromise_factor = 0.4f + (deal.willingness_factor * 0.35f);
	return std::max(compromise_factor, 0.f);
}

void CustomerBrain::playerOfferPenalty(DealData& deal) {
	uint16_t offered_price = deal.item->getCurrentPrice();
	float acceptable = static_cast<float>(deal.acceptable_price);
	float proportional_difference = offered_price / acceptable;
	float offer_penalty = proportional_difference - 1.f;

	if (deal.request == CustomerRequest::Selling) {
		offer_penalty = 1.f - proportional_difference;
	}

	// If new offer is too absurd willingness will reduce
	deal.willingness_factor -= offer_penalty * 0.4f;
}