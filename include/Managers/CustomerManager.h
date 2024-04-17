#pragma once

#include <string>
#include <vector>

#include "Components/Customer.h"
#include "Components/Objects/Item.h"
#include "Components/Renderers/SpriteRenderer.h"

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
	static CustomerManager& getInstance();
	Customer* getCustomer() const;

	// Functions
	void closeDeal();
	void changeCustomer();

private:
	// Constants
	const std::string character_path = "assets/sprites/characters/";
	const std::string customer_head_path = character_path + "customer_head.png";
	const std::string customer_torso_path = character_path + "customer_torso.png";

	const std::string contractor_head_path = character_path + "customer_head.png";
	const std::string contractor_torso_path = character_path + "customer_torso.png";

	// References
	Game& game;

	// Variables
	static CustomerManager* instance;

	Customer* customer = nullptr;
	SpriteRenderer* head_renderer = nullptr;
	SpriteRenderer* torso_renderer = nullptr;

	Object* send_region = nullptr;
	Object* receive_region = nullptr;
	Object* storage = nullptr;

	// Functions
	CustomerRequest generateRequest();
	std::string findBuyRequestItem();
	std::vector<Item*> getStorageItems();
	void createCustomer();
	bool receiveCashForItem();
};

