#include <cstdint>

#include "Components/Collider.h"
#include "Components/Objects/Contract.h"
#include "Components/Renderers/SpriteRenderer.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Core/Utility/RandomGenerator.h"
#include "Managers/ContractManager.h"


ContractManager* ContractManager::instance = nullptr;

//_______________
// Constructors

ContractManager::ContractManager(Game& game) : game(game) {
	if (instance != nullptr) {
		delete this;
		return;
	}
	instance = this;
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
		game, *contract_object, *collider, till_contractor_return
	);
	current_contract = contract;
	return contract;
}

#include <iostream>

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
