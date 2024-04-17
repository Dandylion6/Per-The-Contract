#pragma once

#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>

#include "Components/Objects/Item.h"
#include "Data/ItemData.h"

using json = nlohmann::json;

// Forward declerations
class Game;
class Object;

class ItemFactory
{
public:
	// Constructors
	ItemFactory(Game& game);
	virtual ~ItemFactory();

	// Getters
	static ItemFactory& getInstance();
	ItemData& getItemData(std::string item_id);

	// Functions
	Item* createItem(std::string item_id) const;
	Item* createItem(std::string item_id, Object* parent) const;
	Item* generateRandomItem() const;

private:
	// Constants
	const std::string item_directory = "assets/sprites/objects/items/";
	const std::string json_file_path = "assets/data/item_data_map.json";

	// References
	Game& game;
	
	// Variables
	static ItemFactory* instance;
	std::unordered_map<std::string, std::unique_ptr<ItemData>> item_data_map;

	// Functions
	std::unique_ptr<ItemData> jsonToItemData(std::string item_id, json json);
};