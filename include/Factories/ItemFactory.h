#pragma once

#include <nlohmann/json.hpp>
using json = nlohmann::json;
#include <string>

#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Data/ItemData.h"

class ItemFactory
{
public:
	// Constructors
	ItemFactory(Game& game);
	virtual ~ItemFactory();

	// Functions
	Object* createItem(std::string item_id);
	Object* createItem(std::string item_id, Object* parent);

private:
	// Constants
	const std::string json_file_path = "assets/data/item_data_lookup.json";

	// References
	Game& game;
	
	// Variables
	json item_data_lookup;

	// Functions
	json getItemJson(std::string item_id);
	ItemData jsonToItemData(std::string item_id, json json);
};