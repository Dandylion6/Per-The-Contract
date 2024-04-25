#include <string>

#include "Components/Customer/Brains/CustomerBrain.h"
#include "Core/Managers/Game.h"
#include "Data/DealData.h"
#include "Factories/ItemFactory.h"
#include "Managers/ContractManager.h"


//_______________
// Constructors

CustomerBrain::CustomerBrain(Game& game) : game(game) {
}

CustomerBrain::~CustomerBrain() {
}

std::string CustomerBrain::stateRequest(std::string& insert) {
	switch (game.getDealData()->request) {
	case CustomerRequest::Buying: {
		insert = ItemFactory::getInstance().getItemData(game.getDealData()->request_id).name;
		return "buying";
	}
	case CustomerRequest::Selling:
		return "selling";
	case CustomerRequest::Contract: {
		bool retreiving = ContractManager::getInstance()->isRetrievingContract();
		return retreiving ? "contract_take" : "contract";
	};
	default: break;
	}
}
