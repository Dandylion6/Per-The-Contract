#include <string>

#include "Components/Customer/Brains/CustomerBrain.h"
#include "Core/Managers/Game.h"
#include "Data/DealData.h"
#include "Data/Role.h"
#include "Factories/ItemFactory.h"
#include "Managers/DialogueManager.h"


//_______________
// Constructors

CustomerBrain::CustomerBrain(Game& game) : game(game) {
}

CustomerBrain::~CustomerBrain() {
}

void CustomerBrain::stateRequest() {
	switch (game.getDealData()->request) {
		case CustomerRequest::Buying: {
			std::string insert = ItemFactory::getInstance().getItemData(game.getDealData()->request_id).name;
			DialogueManager::getInstance().generateDialogue(Role::Customer, "buying", insert);
			break;
		}
		case CustomerRequest::Selling: {
			DialogueManager::getInstance().generateDialogue(Role::Customer, "selling");
			break;
		}
	}
	DialogueManager::getInstance().generateDialogue(Role::Customer, "entry_remark");
}
