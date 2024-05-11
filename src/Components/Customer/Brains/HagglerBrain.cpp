#include <cstdint>
#include <memory>
#include <string>

#include "Components/Customer/Brains/CustomerBrain.h"
#include "Components/Customer/Brains/HagglerBrain.h"
#include "Components/Objects/Item.h"
#include "Core/Managers/Game.h"
#include "Core/Utility/Math.h"
#include "Core/Utility/RandomGenerator.h"
#include "Data/DealData.h"
#include "Data/Role.h"
#include "Managers/CustomerManager.h"


//_______________
// Constructors

HagglerBrain::HagglerBrain(Game& game) : CustomerBrain(game) {
}

HagglerBrain::~HagglerBrain() {
}


//___________________
// Public functions

std::string HagglerBrain::actOnPlayerOffer(std::string& insert) {
	std::shared_ptr<DealData> deal_data = game.getDealData();
	Item* item = deal_data->offered_item;
	if (brain_data->perceived_price == 0u) {
		float min = deal_data->request == CustomerRequest::Selling ? 0.75f : 0.95f;
		float max = deal_data->request == CustomerRequest::Selling ? 1.05f : 1.25f;
		uint16_t value = static_cast<uint16_t>(item->getData().market_value * utils::Random::generateFloat(min, max));
		brain_data->perceived_price = ((value + 5u) / 10u) * 10u;
	}

	bool is_selling = deal_data->request == CustomerRequest::Selling;
	uint16_t player_offer = item->getCurrentPrice();

	bool not_negotiating = player_offer == brain_data->last_player_offer;
	if (not_negotiating) brain_data->max_counter_offers = brain_data->counter_offers;
	bool can_counter = brain_data->counter_offers < brain_data->max_counter_offers;
	brain_data->last_player_offer = player_offer;

	float profit = calculateProfit(player_offer, is_selling);
	if (profit >= will_accept_profit && brain_data->counter_offers > 0u) {
		deal_data->customer_accepted_price = std::make_unique<uint16_t>(player_offer);
		return "accept_deal";
	}

	uint16_t counter_price = determineCounterOffer(is_selling);
	if (isGoodOffer(player_offer, counter_price, is_selling)) {
		deal_data->customer_accepted_price = std::make_unique<uint16_t>(player_offer);
		return "accept_deal";
	}

	if (can_counter) {
		brain_data->counter_offers++;
		insert = std::to_string(counter_price);
		deal_data->customer_accepted_price = std::make_unique<uint16_t>(counter_price);
		item->setLatestOfferBy(Role::Customer);
		item->setCurrentPrice(counter_price);
		return "counter_offer";
	}

	if (isAcceptablePrice(player_offer, is_selling)) {
		deal_data->customer_accepted_price = std::make_unique<uint16_t>(player_offer);
		return "accept_deal";
	} else {
		CustomerManager::getInstance().closeDeal();
		return "decline_deal";
	}
}

float HagglerBrain::thinkingTime() {
	return utils::Random::generateFloat(2.5f, 3.f);
}

void HagglerBrain::onEnter() {
	brain_data = std::make_unique<BrainData>();
	brain_data->max_counter_offers = utils::Random::generateInt(least_counter_offers, most_counter_offers);
}

void HagglerBrain::onLeave() {
	brain_data.release();
}

uint16_t HagglerBrain::determineCounterOffer(bool is_selling) {
	uint16_t best_deal = 0u;
	if (is_selling) {
		best_deal = brain_data->perceived_price + static_cast<uint16_t>(brain_data->perceived_price * will_accept_profit);
	} else {
		best_deal = brain_data->perceived_price - static_cast<uint16_t>(brain_data->perceived_price * will_accept_profit);
	}

	float offer_phase = brain_data->counter_offers * negotiation_step;
	uint16_t counteroffer_price = utils::lerp(best_deal, brain_data->perceived_price, offer_phase);
	return ((counteroffer_price + 5u) / 10u) * 10u;
}

uint16_t HagglerBrain::calculateProfit(uint16_t player_offer, bool is_selling) {
	if (is_selling) {
		return (player_offer - brain_data->perceived_price) / brain_data->perceived_price;
	} 
	return (brain_data->perceived_price - player_offer) / brain_data->perceived_price;
}

bool HagglerBrain::isGoodOffer(uint16_t player_offer, uint16_t counter_offer, bool is_selling) {
	return is_selling ? counter_offer <= player_offer : counter_offer >= player_offer;
}

bool HagglerBrain::isAcceptablePrice(uint16_t player_offer, bool is_selling) {
	return is_selling ? player_offer >= brain_data->perceived_price : player_offer <= brain_data->perceived_price;
}
