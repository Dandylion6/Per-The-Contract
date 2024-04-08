#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <vector>

#include "Components/Customer.h"
#include "Components/Renderers/SpriteRenderer.h"

using json = nlohmann::json;

// Forward declerations
class Game;
class Object;

class CustomerManager
{
public:
	// Constructors
	CustomerManager(Game& game);
	virtual ~CustomerManager();

	// Getters
	Customer* getCustomer() const;

	// Functions
	void changeCustomer();
	void letNextCustomerIn();
	void closeShop(bool accepted);

private:
	// Constants
	const std::string character_path = "assets/data/character_list.json";

	// References
	Game& game;

	// Variables
	std::vector<std::shared_ptr<CharacterData>> characters;
	Customer* customer = nullptr;
	SpriteRenderer* head_renderer = nullptr;
	SpriteRenderer* torso_renderer = nullptr;
	Object* send_region = nullptr;
	Object* receive_region = nullptr;

	// Functions
	void createCustomer();
	void loadCharacters();

	bool handleSellRequestClose();
};

