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
	return 0u;
}

void CustomerBrain::determinePerceivedPrice(DealData& deal) {
}

bool CustomerBrain::willAcceptDeal(DealData& deal) {
	return true;
}

bool CustomerBrain::willNegotiate(DealData& deal) {
	return false;
}

bool CustomerBrain::willDeclineDeal(DealData& deal) {
	return false;
}