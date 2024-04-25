#include <string>

#include "Components/Customer/Brains/AssertiveBrain.h"
#include "Components/Customer/Brains/CustomerBrain.h"
#include "Core/Managers/Game.h"


//______________
// Custructors

AssertiveBrain::AssertiveBrain(Game& game) : CustomerBrain(game) {
}

AssertiveBrain::~AssertiveBrain() {
}


//___________________
// Public functions

std::string AssertiveBrain::actOnPlayerOffer(std::string& insert) {
    return "accept_deal";
}
