#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include "Components/Customer/Brains/CustomerBrain.h"
#include "Core/Managers/Game.h"

class FirmBrain : public CustomerBrain {
public:
	// Constructors
	FirmBrain(Game& game);
	virtual ~FirmBrain();

	// Functions
	virtual void stateRequest() override;
	virtual std::string actOnPlayerOffer(std::string& insert) override;
	virtual float thinkingTime() override;
	virtual void onEnter() override;
	virtual void onLeave() override;

private:
	struct BrainData {
		uint16_t perceived_price = 0u;
		uint16_t acceptable_price = 0u;
		bool restated_offer = false;
	};

	// Constants
	float min_price_margin = 0.04f;
	float max_price_margin = 0.12f;

	// Variables
	std::unique_ptr<BrainData> brain_data = nullptr;

	// Functions
	bool isAcceptablePrice(uint16_t player_offer, bool is_selling);
};
