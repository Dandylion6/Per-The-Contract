#pragma once

#include <nlohmann/json.hpp>
using json = nlohmann::json;
#include <string>
#include <map>
#include <memory>

#include "Components/Item.h"
#include "Data/ItemData.h"

// Forward declerations
class Game;
class Object;

class ItemFactory
{
public:
	// Constructors
	ItemFactory(Game& game);
	virtual ~ItemFactory();

	// Functions
	Item* createItem(std::string item_id) const;
	Item* createItem(std::string item_id, Object* parent) const;
	Item* generateRandomItem() const;

private:
	// Constants
	const std::string json_file_path = "assets/data/item_data_map.json";

	// References
	Game& game;
	
	// Variables
	std::unordered_map<std::string, std::unique_ptr<ItemData>> item_data_map;

	// Functions
	std::unique_ptr<ItemData> jsonToItemData(std::string item_id, json json);
};