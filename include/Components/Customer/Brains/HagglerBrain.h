#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include "Components/Customer/Brains/CustomerBrain.h"
#include "Core/Managers/Game.h"

class HagglerBrain : public CustomerBrain {
public:
	// Constructors
	HagglerBrain(Game& game);
	virtual ~HagglerBrain();

	// Functions
	virtual std::string actOnPlayerOffer(std::string& insert) override;
	virtual float thinkingTime() override;
	virtual void onEnter() override;
	virtual void onLeave() override;

private:
	struct BrainData {
		uint16_t perceived_price = 0u;
		uint16_t last_player_offer = 0u;
		uint8_t counter_offers = 0u;
		uint8_t max_counter_offers = 0u;
	};

	// Constants
	float will_accept_profit = 0.3f;
	float negotiation_step = 0.25f;
	uint8_t least_counter_offers = 2u;
	uint8_t most_counter_offers = 4u;

	// Variables
	std::unique_ptr<BrainData> brain_data = nullptr;

	// Functions
	virtual uint16_t determineCounterOffer(bool is_selling) override;
	uint16_t calculateProfit(uint16_t player_offer, bool is_selling);
	bool isGoodOffer(uint16_t player_offer, uint16_t counter_offer, bool is_selling);
	bool isAcceptablePrice(uint16_t player_offer, bool is_selling);
};

