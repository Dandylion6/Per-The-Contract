#include <fstream>
#include <nlohmann/json.hpp>
#include <string>

#include "Components/Collider.h"
#include "Components/Drag.h"
#include "Components/Renderer/SpriteRenderer.h"
#include "Core/Managers/Game.h"
#include "Data/ItemData.h"
#include "Factories/ItemFactory.h"


//_______________
// Constructors

ItemFactory::ItemFactory(Game& game) : game(game) {
	std::ifstream stream(json_file_path);
	item_data_lookup = json::parse(stream);
}

ItemFactory::~ItemFactory() {
}


//___________________
// Public functions

Object* ItemFactory::createItem(std::string item_id) {
	return createItem(item_id, nullptr);
}

Object* ItemFactory::createItem(std::string item_id, Object* parent) {
	json json_data = getItemJson(item_id);
	if (json_data.empty()) return nullptr; // Can't make an item
	
	ItemData item_data = jsonToItemData(item_id, json_data);
	Object* object = new Object(game, item_id, parent);

	// Add item components
	SpriteRenderer* renderer = new SpriteRenderer(
		game, *object, item_data.sprite_path
	);
	Collider* collider = new Collider(
		game, *object, renderer->getSize(), Layer::Item
	);
	Drag* drag = new Drag(
		game, *object, *collider, Layer::ItemDrop
	);

	return object;
}


//____________________
// Private functions

json ItemFactory::getItemJson(std::string item_id) {
	auto it = item_data_lookup.find(item_id);
	if (it != item_data_lookup.end()) {
		return it.value();
	}
	return json();
}

ItemData ItemFactory::jsonToItemData(std::string item_id, json json) {
	return ItemData(
		item_id,
		json["name"],
		json["sprite_path"],
		json["market_value"]
	);
}
