#pragma once

#include <random>
#include <string>
#include <vector>

#include "Components/CustomerAnimator.h"
#include "Core/Component.h"
#include "Core/Utility/Vector2.h"
#include "Data/CharacterData.h"
#include "Data/ItemData.h"
#include "Managers/DialogueManager.h"

// Forward declerations
class Game;
class Object;

enum RequestType
{
	Buy, Sell, Trade
};

class Customer : public Component
{
public:
	// Constructors
	Customer(Game& game, Object& object);
	virtual ~Customer();

	// Setters
	void setFunds(uint16_t funds);
	void setCharacter(std::weak_ptr<CharacterData> character);

	// Functions
	void addToInventory(std::string item);
	void enter();
	void generateRequest();
	void placeSellOffer();
	void leave();
	void update(float delta_time) override;

private:
	// Constant
	uint8_t drop_range = 70u;

	// References
	DialogueManager& dialogue_manager;

	// Variables
	std::weak_ptr<CharacterData> character;
	CustomerAnimator* animator;
	std::mt19937 random_generator;

	std::vector<std::string> inventory;
	uint16_t funds = 0u;
	RequestType* request_type = nullptr;
};

