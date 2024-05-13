#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>

#include "Components/Objects/Item.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Data/ItemData.h"

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
	const std::map<float, uint8_t> rarity_map{
		{ 57.f, 0u }, // Abundant
		{ 31.f, 1u }, // Frequent
		{ 9.f, 2u }, // Scarce
		{ 3.f, 3u }   // Rare
	};

	// References
	Game& game;
	
	// Variables
	static ItemFactory* instance;
	std::unordered_map<std::string, std::unique_ptr<ItemData>> item_data_map;

	// Functions
	std::unique_ptr<ItemData> jsonToItemData(std::string item_id, nlohmann::json json);
	std::unordered_map<std::string, std::unique_ptr<ItemData>> getRandomRarityPool() const;
};