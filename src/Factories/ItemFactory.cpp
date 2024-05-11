#include <cstdint>
#include <fstream>
#include <iosfwd>
#include <memory>
#include <string>
#include <utility>

#include "Components/Collider.h"
#include "Components/Objects/Item.h"
#include "Components/Renderers/SpriteRenderer.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Core/Utility/RandomGenerator.h"
#include "Data/ItemData.h"
#include "Factories/ItemFactory.h"
#include "nlohmann/json.hpp"
#include <unordered_map>


ItemFactory* ItemFactory::instance = nullptr;

//_______________
// Constructors

ItemFactory::ItemFactory(Game& game) : game(game) {
	if (instance != nullptr) {
		delete this;
		return;
	}
	instance = this;

	std::ifstream stream(json_file_path);
	nlohmann::json item_data_json = nlohmann::json::parse(stream);

	// Convert json to item data map
	for (auto it = item_data_json.begin(); it != item_data_json.end(); ++it) {
		const std::string& item_id = it.key();
		item_data_map[item_id] = jsonToItemData(item_id, it.value());
	}
}

ItemFactory::~ItemFactory() {
}


//__________
// Getters

ItemFactory& ItemFactory::getInstance() {
	return *instance;
}

ItemData& ItemFactory::getItemData(std::string item_id) {
	return *(item_data_map.find(item_id)->second);
}


//___________________
// Public functions

Item* ItemFactory::createItem(std::string item_id) const {
	return createItem(item_id, nullptr);
}

Item* ItemFactory::createItem(std::string item_id, Object* parent) const {
	auto it = item_data_map.find(item_id);
	if (it == item_data_map.end()) return nullptr; // Couldn't find item

	Object* object = new Object(game, item_id, parent);
	ItemData& item_data = *(it->second);

	// Add item components
	SpriteRenderer* renderer = new SpriteRenderer(
		game, *object, item_directory + item_data.sprite_path
	);
	Collider* collider = new Collider(game, *object, renderer->getSize());
	return new Item(game, *object, *collider, item_data);
}

Item* ItemFactory::generateRandomItem() const {
	auto item_map = getRandomRarityPool();
	size_t map_size = item_map.size();

	int random_index = utils::Random::randomIndex(map_size);
	auto it = std::next(item_map.begin(), random_index);
	return createItem(it->first); // Create item based on random item id
}


//____________________
// Private functions

std::unique_ptr<ItemData> ItemFactory::jsonToItemData(
	std::string item_id, nlohmann::json json
) {
	return std::make_unique<ItemData>(
		item_id,
		json["name"],
		json["sprite_path"],
		json["market_value"],
		json["rarity"]
	);
}

std::unordered_map<std::string, std::unique_ptr<ItemData>> ItemFactory::getRandomRarityPool() const {
	float random_rarity = utils::Random::generateFloat(0.f, 100.f);
	uint8_t rarity = 0u;
	float ori = random_rarity;
	for (auto it = rarity_map.rbegin(); it != rarity_map.rend(); ++it) {
		std::pair<float, uint8_t> pair = *it;
		if (random_rarity <= pair.first) {
			rarity = pair.second;
			break;
		}
		random_rarity -= pair.first;
	}

	std::unordered_map<std::string, std::unique_ptr<ItemData>> rarity_pool;
	for (const auto& pair : item_data_map) {
		const auto& item_data = pair.second;
		if (item_data->rarity == rarity) {
			rarity_pool.emplace(pair.first, std::make_unique<ItemData>(*item_data));
		}
	}
	return rarity_pool;
}
