#include <fstream>
#include <memory>
#include <nlohmann/json.hpp>

#include "Components/Customer.h"
#include "Core/Object.h"
#include "Core/Utility/RandomGenerator.h"
#include "Data/CharacterData.h"
#include "Factories/ItemFactory.h"
#include "Managers/CustomerManager.h"

//_______________
// Constructors

CustomerManager::CustomerManager(Game& game) : game(game) {
	createCustomer();
	loadCharacters();
}

CustomerManager::~CustomerManager() {
}


//__________
// Getters

Customer* CustomerManager::getCustomer() const {
	return this->customer;
}


//___________________
// Public functions

void CustomerManager::changeCustomer() {
	// Get random character
	std::shared_ptr<CharacterData> random_character;
	int random_index = utils::RandomGenerator::randomIndex(characters.size());

	random_character = characters[random_index];
	customer->setCharacter(random_character);

	// Change sprites
	torso_renderer->setSprite(random_character->torso_file_path);
	head_renderer->setSprite(random_character->head_file_path);
	
	// Generate customer
	customer->setCustomer(
		static_cast<CustomerTrait>(utils::RandomGenerator::randomIndex(6)),
		utils::RandomGenerator::generateInt(10, 50) * 10u,
		utils::RandomGenerator::generateFloat(0.5f, 1.f)
	);
	customer->enter();
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

