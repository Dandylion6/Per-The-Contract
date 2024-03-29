#include <fstream>
#include <map>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>

#include "Components/Collider.h"
#include "Components/Item.h"
#include "Components/Renderers/SpriteRenderer.h"
#include "Core/Managers/Game.h"
#include "Core/Utility/RandomGenerator.h"
#include "Data/ItemData.h"
#include "Factories/ItemFactory.h"


//_______________
// Constructors

ItemFactory::ItemFactory(Game& game) : game(game) {
	std::ifstream stream(json_file_path);
	json item_data_json = json::parse(stream);

	// Convert json to item data map
	for (auto it = item_data_json.begin(); it != item_data_json.end(); ++it) {
		const std::string& item_id = it.key();
		item_data_map[item_id] = jsonToItemData(item_id, it.value());
	}
}

ItemFactory::~ItemFactory() {
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
		game, *object, item_data.sprite_path
	);
	Collider* collider = new Collider(game, *object, renderer->getSize());
	return new Item(game, *object, *collider, item_data);
}

Item* ItemFactory::generateRandomItem() const {
	size_t map_size = item_data_map.size();
	int random_index = utils::RandomGenerator::randomIndex(map_size);
	auto it = std::next(item_data_map.begin(), random_index);
	return createItem(it->first); // Create item based on random item id
}


//____________________
// Private functions

std::unique_ptr<ItemData> ItemFactory::jsonToItemData(
	std::string item_id, json json
) {
	return std::make_unique<ItemData>(
		item_id,
		json["name"],
		json["sprite_path"],
		json["market_value"]
	);
}
