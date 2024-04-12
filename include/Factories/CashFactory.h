#pragma once

#include <nlohmann/json.hpp>
#include <stdint.h>
#include <string>
#include <unordered_map>
#include <vector>

#include "Components/Cash.h"

using json = nlohmann::json;

// Forward declerations
class Game;

class CashFactory
{
public:
	// Constructors
	CashFactory(Game& game);
	virtual ~CashFactory();

	// Getters
	static CashFactory& getInstance();

	// Functions
	std::vector<Cash*> createCash(uint16_t value);

private:
	// Constants
	const std::string sprite_directory = "assets/sprites/objects/cash/";
	const std::vector<uint8_t> cash_values {
		200u, 100u, 50, 20, 10u
	};

	// References
	Game& game;

	// Variables
	static CashFactory* instance;

	// Functions
	std::vector<uint8_t> splitValue(uint16_t value);
};