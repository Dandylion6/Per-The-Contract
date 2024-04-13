#include <fstream>
#include <memory>
#include <nlohmann/json.hpp>
#include <stdint.h>
#include <vector>

#include "Components/Customer.h"
#include "Core/Object.h"
#include "Core/Utility/RandomGenerator.h"
#include "Data/CharacterData.h"
#include "Factories/ItemFactory.h"
#include "Managers/CustomerManager.h"
#include "Managers/DialogueManager.h"
#include "Components/Cash.h"
#include "Components/Item.h"
#include "Factories/CashFactory.h"
#include "Core/Utility/Vector2.h"
#include "Data/DealData.h"


CustomerManager* CustomerManager::instance = nullptr;

//_______________
// Constructors

CustomerManager::CustomerManager(Game& game) : game(game) {
	if (instance != nullptr) {
		delete this;
		return;
	}
	instance = this;

	createCustomer();
	loadCharacters();

	send_region = game.getObject("send_region");
	receive_region = game.getObject("receive_region");
}

CustomerManager::~CustomerManager() {
}


//__________
// Getters

CustomerManager& CustomerManager::getInstance() {
	return *instance;
}

Customer* CustomerManager::getCustomer() const {
	return this->customer;
}


//___________________
// Public functions

void CustomerManager::changeCustomer() {
	// Get random character
	std::shared_ptr<CharacterData> random_character;
	int random_index = utils::Random::randomIndex(characters.size());

	random_character = characters[random_index];
	customer->setCharacter(random_character);

	// Change sprites
	torso_renderer->setSprite(random_character->torso_file_path);
	head_renderer->setSprite(random_character->head_file_path);
	
	// Generate customer
	customer->setCustomer(
		static_cast<CustomerTrait>(utils::Random::randomIndex(6)),
		utils::Random::generateInt(10, 50) * 10u
	);
	customer->enter();
}

void CustomerManager::letNextCustomerIn() {
	// Can't let next customer in if still in deal
	if (game.getDealData() != nullptr) return;

	// If items aren't yet put into inventory then can't let customer in
	if (send_region->getChildren().size() > 0u) return;
	if (receive_region->getChildren().size() > 0u) return;

	// Generate new customer
	changeCustomer();
}

void CustomerManager::closeShop(bool accepted) {
	// Check when accepting if shop can be closed
	if (accepted) {
		CustomerRequest request = game.getDealData()->request;
		if (request == CustomerRequest::Selling) {
			if (!handleSellRequestClose()) return;
		}

	}
	game.setDealData(nullptr); // Release current deal
	DialogueManager::getInstance().clearDialogue();

	// TODO: Animate closing
	letNextCustomerIn();
}


//____________________
// Private functions

void CustomerManager::createCustomer() {
	// Instantiate objects
	Object* customer_object = new Object(game, "customer");
	Object* head_object = new Object(game, "head", customer_object);
	Object* torso_object = new Object(game, "torso", customer_object);

	customer_object->setPosition(Vector2(300.f, 800.f));
	torso_object->setZIndex(-1);
	torso_object->setAnchor(Vector2(0.5f, 1.f));
	head_object->setAnchor(Vector2(0.5f, 1.f));

	// Add components
	torso_renderer = new SpriteRenderer(game, *torso_object);
	head_renderer = new SpriteRenderer(game, *head_object);
	customer = new Customer(game, *customer_object);
}

void CustomerManager::loadCharacters() {
	std::ifstream character_stream(character_path);
	json data = json::parse(character_stream);

	// Create characters from json
	for (auto it = data.begin(); it != data.end(); ++it) {
		const json& character = it.value();
		std::shared_ptr<CharacterData> character_data;
		character_data = std::make_shared<CharacterData>(
			"name",
			character["head"],
			character["body"],
			character["gender"]
		);
		characters.push_back(character_data);
	}
}

bool CustomerManager::handleSellRequestClose() {
	// Get the total needed cash
	uint16_t total_cash_needed = 0u;
	for (Object* object : receive_region->getChildren()) {
		Item* item = object->getComponent<Item>();
		if (item == nullptr) continue;
		total_cash_needed += item->getCurrentPrice();
	}
	
	// Get cash deposited
	uint16_t cash_deposited = 0u;
	std::vector<Cash*> cash_to_give;

	for (Object* object : send_region->getChildren()) {
		Cash* cash = object->getComponent<Cash>();
		if (cash == nullptr) continue;

		cash_deposited += cash->getValue();
		cash_to_give.push_back(cash);
		if (cash_deposited >= total_cash_needed) break; // Exit if payment is met
	}

	// Can't pay yet
	if (cash_deposited < total_cash_needed) return false;

	// Give customer owed cash
	for (Cash* cash : cash_to_give) game.deleteObject(&cash->getObject());
	cash_to_give.clear();

	// Transfer ownership of items
	for (Object* object : receive_region->getChildren()) {
		Item* item = object->getComponent<Item>();
		if (item == nullptr) continue;
		item->setOwned(true); // Player now owns the item
	}

	// Get change back if needed
	if (cash_deposited > total_cash_needed) {
		uint16_t change_value = cash_deposited - total_cash_needed;
		const std::vector<Cash*>& change = CashFactory::getInstance().createCash(change_value);
		for (Cash* cash : change) {
			Object& cash_object = cash->getObject();
			cash_object.setParent(receive_region);

			int x = utils::Random::generateInt(-80, 80);
			int y = utils::Random::generateInt(-80, 80);
			cash_object.setLocalPosition(Vector2(x, y));
		}
	}
	return true;
}

