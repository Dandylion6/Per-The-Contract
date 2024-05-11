#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

#include "Components/Objects/Contract.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Data/OrderType.h"

class ContractManager {
public:
	// Constructors
	ContractManager(Game& game);
	virtual ~ContractManager();

	// Getters
	static ContractManager* getInstance();
	Contract* getCurrentContract() const;
	std::string getContractLine(OrderType order_type);

	// Functions
	bool isContractorEntering() const;
	bool isRetrievingContract() const;
	bool isContractComplete();

	Contract* generateContract();
	void contractorArriveCheck();
	void contractorLeave();

private:
	// Constants
	std::string line_path = "assets/data/contract_orders.json";
	std::string object_sprite_path = "assets/sprites/objects/contract_order.png";
	std::unordered_map<std::string, OrderType> order_type_map{
		{ "funding", OrderType::Funding }
	};

	const uint8_t min_hours_waiting = 9u;
	const uint8_t max_hours_waiting = 18u;
	const uint8_t min_hours_away = 12u;
	const uint8_t max_hours_away = 24u;

	// References
	Game& game;

	// Variables
	static ContractManager* instance;
	Contract* current_contract = nullptr;
	bool contractor_arrived = false;
	bool retreiving_contract = false;
	std::unordered_map<OrderType, std::string> contract_lines;

	Object* send_region = nullptr;

	uint8_t contractor_left_time = 0u;
	uint8_t till_contractor_return = 0u;
};

