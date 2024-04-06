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
	uint16_t offered_price = deal.item->getCurrentPrice();
	float base_factor = determineBasePriceChange(deal);
	float price_change_factor = base_factor;

	// Negotiators are willing to push the price more
	bool first_offer = deal.item->getLastPrice() == 0u;
	float first_offer_factor = deal.negotiability * 0.3f * base_factor;
	price_change_factor += first_offer ? first_offer_factor * 0.15f : 0.f;
	price_change_factor += deal.negotiability * base_factor * 0.4f;

	// Random adjustment
	float randomness = utils::Random::generateFloat(-0.2f, 0.25f);
	price_change_factor += randomness * base_factor * 0.3f;

	uint16_t price_change = static_cast<uint16_t>(
		offered_price * price_change_factor
	);
	uint16_t new_offer = finilisePriceChange(deal, price_change);
	return ((new_offer + 5u) / 10u) * 10u; // Round last number
}

void CustomerBrain::determinePerceivedPrice(DealData& deal) {
	// TODO: Generate perceived price
	// Knowledgeable trait makes this more accurate
	deal.perceived_item_value = deal.item->getData().market_value;
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
	deal.willingness -= offer_penalty * 0.4f;
	deal.negotiability -= 0.1f; // Reduce negotiability
}

bool CustomerBrain::isAcceptablePrice(DealData& deal) {
	uint16_t offered_price = deal.item->getCurrentPrice();
	if (deal.request == CustomerRequest::Selling) {
		return offered_price >= calculateMinPrice(deal);
	}
	return offered_price <= calculateMaxPrice(deal);
}

bool CustomerBrain::willAcceptDeal(DealData& deal) {
	// Certain customers might be more willing to accept or negotiate futher
	float accept_chance = 1.f - (deal.negotiability * 0.6f);
	float accept_value = utils::Random::generateFloat(0.f, accept_chance);

	// Low negotiability are way more inclined to accept
	float acceptance_threshold = std::powf(deal.negotiability - 0.3f, 0.4f);

	// High willingness leads to lower acceptance threshold
	acceptance_threshold -= deal.willingness * 0.4;
	return accept_value > acceptance_threshold;
}

bool CustomerBrain::willNegotiate(DealData& deal) {
	// The chance at negotiating influenced by willingness
	float negotiate_chance = deal.willingness * 0.8f;
	float negotiate_value = utils::Random::generateFloat(0.f, negotiate_chance);

	// A high negiability is almost garanteed to negotiate
	float negotiation_threshold = 1.f - deal.negotiability * 0.7f;
	return negotiate_value > negotiation_threshold;
}

bool CustomerBrain::willDeclineDeal(DealData& deal) {
	return deal.willingness < 0.35f;
}


//____________________
// Private functions

uint16_t CustomerBrain::finilisePriceChange(DealData& deal, uint16_t change) {
	uint16_t offered_price = deal.item->getCurrentPrice();
	if (deal.request == CustomerRequest::Selling) {
		// Can't be less than minimum price
		return std::max(
			static_cast<uint16_t>(offered_price + change),
			calculateMinPrice(deal)
		);
	} else {
		// Can't exceed funds or max price when buying 
		uint16_t new_offer = offered_price - change;
		new_offer = std::min(new_offer, calculateMaxPrice(deal));
		return new_offer < deal.funds ? deal.funds : new_offer;
	}
}

uint16_t CustomerBrain::calculateMinPrice(DealData& deal) {
	uint16_t range = static_cast<uint16_t>(
		deal.perceived_item_value * deal.acceptable_range
	);
	return deal.perceived_item_value - range;
}

uint16_t CustomerBrain::calculateMaxPrice(DealData& deal) {
	uint16_t range = static_cast<uint16_t>(
		deal.perceived_item_value * deal.acceptable_range
	);
	return deal.perceived_item_value + range;
}

float CustomerBrain::determineBasePriceChange(DealData& deal) {
	float offered_price = static_cast<float>(deal.item->getCurrentPrice());
	float base_change = 0.f;

	// Adjust base price change factor based on perceived price
	float perceived_price_factor = static_cast<float>(
		deal.perceived_item_value / offered_price
	);
	base_change += perceived_price_factor - 1.f;

	// Try making a compormise based on willingness
	float compromise_factor = 0.15f + (deal.willingness * 0.35f);
	return base_change - std::max(compromise_factor, 0.f);
}