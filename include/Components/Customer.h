#pragma once

#include <string>
#include <vector>

#include "Components/CustomerAnimator.h"
#include "Components/Renderers/SpriteRenderer.h"
#include "Core/Component.h"
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
	// References
	DialogueManager& dialogue_manager;

	// Variables
	std::weak_ptr<CharacterData> character;
	CustomerAnimator* animator;

	std::vector<std::string> inventory;
	uint16_t funds = 0u;
	RequestType* request_type = nullptr;
};

