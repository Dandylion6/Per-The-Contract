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
	Contract* getCurrentContract() const;
	bool isContractorEntering() const;
	bool isRetrievingContract() const;

	// Functions
	Contract* generateContract();
	void contractorArriveCheck();
	void contractorLeave();

private:
	const uint8_t min_hours_waiting = 4u;
	const uint8_t max_hours_waiting = 8u;
	const uint8_t min_hours_away = 12u;
	const uint8_t max_hours_away = 24u;

	// References
	Game& game;

	// Variables
	static ContractManager* instance;
	Contract* current_contract = nullptr;
	bool contractor_arrived = false;
	bool retreiving_contract = false;

	uint8_t contractor_left_time = 0u;
	uint8_t till_contractor_return = 0u;
};

