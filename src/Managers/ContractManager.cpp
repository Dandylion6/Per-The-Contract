#include <cstdint>

#include "Components/Collider.h"
#include "Components/Objects/Cash.h"
#include "Components/Objects/Contract.h"
#include "Components/Renderers/SpriteRenderer.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Core/Utility/RandomGenerator.h"
#include "Data/OrderType.h"
#include "Managers/ContractManager.h"
#include <vector>


ContractManager* ContractManager::instance = nullptr;

//_______________
// Constructors

ContractManager::ContractManager(Game& game) : game(game) {
	if (instance != nullptr) {
		delete this;
		return;
	}
	instance = this;
	send_region = game.getObject("send_region");
	contractorLeave();
}

ContractManager::~ContractManager() {
}


//__________
// Getters

ContractManager* ContractManager::getInstance() {
	return instance;
}

Contract* ContractManager::getCurrentContract() const {
	return this->current_contract;
}

bool ContractManager::isRetrievingContract() const {
	return this->retreiving_contract;
}

bool ContractManager::isContractComplete() const {
	if (current_contract == nullptr) return false;
	if (&current_contract->getObject() != send_region) return false;
	if (current_contract->getOrderType() == OrderType::Funding) {
		uint16_t cash_deposited = 0u;
		std::vector<Cash*> all_cash;
		for (Object* child : send_region->getChildren()) {
			Cash* cash = child->getComponent<Cash>();
			if (cash == nullptr) continue;
			cash_deposited += cash->getValue();
			all_cash.push_back(cash);
		}
		if (cash_deposited < 200u) return false;
		for (Cash* cash : all_cash) {
			game.deleteObject(&cash->getObject());
		}
		game.deleteObject(&current_contract->getObject());
		return true;
	}
	return false;
}

bool ContractManager::isContractorEntering() const {
	return this->contractor_arrived;
}


//___________________
// Public functions

Contract* ContractManager::generateContract() {
	if (current_contract != nullptr) return nullptr;

	Object* contract_object = new Object(game, "contract");
	SpriteRenderer* renderer = new SpriteRenderer(game, *contract_object, "");
	Collider* collider = new Collider(game, *contract_object, renderer->getSize());

	till_contractor_return = static_cast<uint8_t>(
		utils::Random::generateInt(min_hours_away, max_hours_away)
	);

	Contract* contract = new Contract(
		game, *contract_object, *collider, till_contractor_return, OrderType::Funding
	);
	current_contract = contract;
	return contract;
}

void ContractManager::contractorArriveCheck() {
	if (contractor_arrived) return;
	uint8_t hours_away = (game.getTimeOfDay() - contractor_left_time) % 24u;
	if (hours_away >= till_contractor_return) {
		retreiving_contract = current_contract != nullptr;
		contractor_arrived = true;
	}
}

void ContractManager::contractorLeave() {
	contractor_left_time = game.getTimeOfDay();
	if (current_contract == nullptr) {
		till_contractor_return = static_cast<uint8_t>(
			utils::Random::generateInt(min_hours_waiting, max_hours_waiting)
		);
	}
	contractor_arrived = false;
}
