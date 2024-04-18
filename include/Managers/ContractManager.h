#pragma once

#include <cstdint>

#include "Components/Objects/Contract.h"
#include "Core/Managers/Game.h"

class ContractManager {
public:
	// Constructors
	ContractManager(Game& game);
	virtual ~ContractManager();

	// Getters
	static ContractManager* getInstance();
	bool isRetrievingContract() const;
	Contract* getCurrentContract() const;

	// Functions
	Contract* generateContract();
	bool isContractorEntering();
	void contractorLeave();

private:
	const uint8_t min_hours_waiting = 6u;
	const uint8_t max_hours_waiting = 12u;
	const uint8_t min_hours_away = 10u;
	const uint8_t max_hours_away = 20u;

	// References
	Game& game;

	// Variables
	static ContractManager* instance;
	Contract* current_contract = nullptr;
	bool retreiving_contract = false;

	uint8_t contractor_left_time = 0u;
	uint8_t till_contractor_return = 0u;
};

