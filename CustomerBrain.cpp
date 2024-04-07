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
	float price_factor = 1.f;

	bool first_offer = deal.item->getLastPrice() == 0u;
	float negotiation_factor = std::powf(deal.negotiability - 0.2f, 0.5f);
	price_factor += negotiation_factor;

	// Random adjustment
	float randomness = utils::Random::generateFloat(-0.15f, 0.2f);
	price_factor = std::max(price_factor + randomness, 1.f);

	uint16_t new_offer = offered_price * base_factor * price_factor;
	new_offer = finalizePriceOffer(deal, new_offer);
	return ((new_offer + 5u) / 10u) * 10u; // Round last number
}

void CustomerBrain::determinePerceivedPrice(DealData& deal) {
	// TODO: Generate perceived price
	// Knowledgeable trait makes this more accurate
	deal.perceived_item_value = deal.item->getData().market_value;
}

void CustomerBrain::playerOfferPenalty(DealData& deal) {
	float offered_price = static_cast<float>(deal.item->getCurrentPrice());
	float percived_value = static_cast<float>(deal.perceived_item_value);

	float proportional_difference = percived_value / offered_price;
	if (deal.request == CustomerRequest::Selling) {
		proportional_difference = offered_price / percived_value;
	}
	float offer_penalty = 1.f - proportional_difference;
	offer_penalty = std::min(offer_penalty, 0.6f);

	// If new offer is too absurd reduction
	deal.willingness -= offer_penalty * 0.4f;
	deal.negotiability -= std::max(offer_penalty * 0.2f, 0.05f);
}

bool CustomerBrain::isAcceptablePrice(DealData& deal) {
	uint16_t offered_price = deal.item->getCurrentPrice();
	if (deal.request == CustomerRequest::Selling) {
		return offered_price >= calculateMinPrice(deal);
	}
	return offered_price <= calculateMaxPrice(deal);
}

bool CustomerBrain::willAcceptDeal(DealData& deal) {
	// Low negotiability are way more inclined to accept
	float accept_value = utils::Random::generateFloat(0.0f, 0.7f);
	float acceptance_threshold = std::powf(deal.negotiability, 1.6f);
	return accept_value > acceptance_threshold;
}

bool CustomerBrain::willNegotiate(DealData& deal) {
	// The less willing the higher the negotiation threshold is
	float negotiate_value = utils::Random::generateFloat(0.f, 0.8f);
	float negotiation_threshold = std::powf(1.1f - deal.willingness, 2.f);

	// A high negiability will lead to higher probability to negotiating
	negotiation_threshold += 0.5f - deal.negotiability;
	return negotiate_value > negotiation_threshold;
}

bool CustomerBrain::willDeclineDeal(DealData& deal) {
	return deal.willingness < 0.1f;
}


//____________________
// Private functions

uint16_t CustomerBrain::finalizePriceOffer(DealData& deal, uint16_t offer) {
	uint16_t offered_price = deal.item->getCurrentPrice();
	if (deal.request == CustomerRequest::Selling) {
		// Can't be less than minimum price
		return std::max(offer, calculateMinPrice(deal));
	}
	// Can't exceed funds or max price when buying 
	uint16_t new_offer = std::min(offer, calculateMaxPrice(deal));
	return new_offer < deal.funds ? deal.funds : new_offer;
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
	if (deal.request == CustomerRequest::Selling) {
		uint16_t target_value = calculateMinPrice(deal);
		return static_cast<float>(target_value / offered_price);
	}
	uint16_t target_value = calculateMaxPrice(deal);
	return static_cast<float>(offered_price / target_value);
}