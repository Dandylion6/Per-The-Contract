#pragma once

#include <cstdint>
#include <string>

#include "Core/Managers/Game.h"

class CustomerBrain {
public:
	// Constructors
	CustomerBrain(Game& game);
	virtual ~CustomerBrain();

	// Functions
	virtual std::string stateRequest(std::string& insert);
	virtual std::string actOnPlayerOffer(std::string& insert) = 0;
	virtual float thinkingTime() = 0;
	virtual void onEnter() = 0;
	virtual void onLeave() = 0;
	
protected:
	// References
	Game& game;

	// Functions
	virtual uint16_t determineCounterOffer(bool is_selling) = 0;
};