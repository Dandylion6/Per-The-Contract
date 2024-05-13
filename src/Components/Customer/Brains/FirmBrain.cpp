#include <cstdint>
#include <memory>
#include <string>

#include "Components/Customer/Brains/CustomerBrain.h"
#include "Components/Customer/Brains/FirmBrain.h"
#include "Core/Managers/Game.h"
#include "Core/Utility/RandomGenerator.h"
#include "Data/DealData.h"
#include "Data/Role.h"
#include "Factories/ItemFactory.h"
#include "Managers/DialogueManager.h"


//_______________
// Constructors

FirmBrain::FirmBrain(Game& game) : CustomerBrain(game) {
}

FirmBrain::~FirmBrain() {
}


//___________________
// Public functions

void FirmBrain::stateRequest() {
	CustomerBrain::stateRequest();
	DialogueManager::getInstance().generateDialogue(Role::Customer, "initial_offer", std::to_string(brain_data->perceived_price));
	std::shared_ptr<DealData> deal_data = game.getDealData();
	if (deal_data->request == CustomerRequest::Selling) {
		deal_data->offered_item->setCurrentPrice(brain_data->perceived_price);
		deal_data->customer_accepted_price = std::make_unique<uint16_t>(brain_data->perceived_price);
		deal_data->offered_item->setLatestOfferBy(Role::Customer);
	}
}

std::string FirmBrain::actOnPlayerOffer(std::string& insert) {
	if (brain_data->restated_offer) {
		return "decline_deal";
	}

	std::shared_ptr<DealData> deal_data = game.getDealData();
	uint16_t player_offer = deal_data->offered_item->getCurrentPrice();
	bool is_selling = deal_data->request == CustomerRequest::Selling;
	if (isAcceptablePrice(player_offer, is_selling)) {
		deal_data->customer_accepted_price = std::make_unique<uint16_t>(player_offer);
		return "accept_deal";
	}
	deal_data->offered_item->setCurrentPrice(brain_data->perceived_price);
	deal_data->offered_item->setLatestOfferBy(Role::Customer);
	brain_data->restated_offer = true;

	insert = std::to_string(brain_data->perceived_price);
	return "hold_offer";
}

float FirmBrain::thinkingTime() {
	return brain_data->restated_offer ? utils::Random::generateFloat(0.8f, 1.2f) : utils::Random::generateFloat(1.8f, 2.5f);
}

void FirmBrain::onEnter() {
	brain_data = std::make_unique<BrainData>();
	std::shared_ptr<DealData> deal_data = game.getDealData();
	if (deal_data->request == CustomerRequest::Contract) return;
	bool is_selling = deal_data->request == CustomerRequest::Selling;
	float min = is_selling ? 0.8f : 0.95f;
	float max = is_selling ? 1.05f : 1.2f;

	uint16_t true_value = ItemFactory::getInstance().getItemData(deal_data->request_id).market_value;
	uint16_t value = static_cast<uint16_t>(true_value * utils::Random::generateFloat(min, max));
	brain_data->perceived_price = ((value + 5u) / 10u) * 10u;

	float price_margin = utils::Random::generateFloat(min_price_margin, max_price_margin);
	uint16_t margin = static_cast<uint16_t>(brain_data->perceived_price * price_margin);
	brain_data->acceptable_price = is_selling ? brain_data->perceived_price - margin : brain_data->perceived_price + margin;
}

void FirmBrain::onLeave() {
	brain_data.release();
}


//____________________
// Private functions

bool FirmBrain::isAcceptablePrice(uint16_t player_offer, bool is_selling) {
	if (is_selling) return player_offer >= brain_data->acceptable_price;
	return player_offer <= brain_data->acceptable_price;
}
