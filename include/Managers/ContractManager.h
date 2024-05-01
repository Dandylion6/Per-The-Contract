#pragma once

#include <cstdint>

#include "Components/Objects/Contract.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"

class ContractManager {
public:
	// Constructors
	ContractManager(Game& game);
	virtual ~ContractManager();

	// Getters
	static ContractManager* getInstance();
	Contract* getCurrentContract() const;

	// Functions
	bool isContractorEntering() const;
	bool isRetrievingContract() const;
	bool isContractComplete() const;

	Contract* generateContract();
	void contractorArriveCheck();
	void contractorLeave();

private:
	const uint8_t min_hours_waiting = 0u;
	const uint8_t max_hours_waiting = 0u;
	const uint8_t min_hours_away = 12u;
	const uint8_t max_hours_away = 24u;

	// References
	Game& game;

	// Variables
	static ContractManager* instance;
	Contract* current_contract = nullptr;
	bool contractor_arrived = false;
	bool retreiving_contract = false;

	Object* send_region = nullptr;

	uint8_t contractor_left_time = 0u;
	uint8_t till_contractor_return = 0u;
};

