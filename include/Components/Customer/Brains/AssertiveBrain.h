#pragma once

#include <string>

#include "Components/Customer/Brains/CustomerBrain.h"
#include "Core/Managers/Game.h"

class AssertiveBrain : public CustomerBrain {
public:
	// Constructors
	AssertiveBrain(Game& game);
	virtual ~AssertiveBrain();

	// Functions
	virtual std::string actOnPlayerOffer(std::string& insert) override;
};

