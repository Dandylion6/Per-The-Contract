#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Components/CustomerAnimator.h"
#include "Components/Item.h"
#include "Core/Component.h"
#include "Core/Utility/Vector2.h"
#include "Data/CharacterData.h"
#include "Data/CustomerTrait.h"
#include "Data/ItemData.h"
#include "Managers/DialogueManager.h"
#include "Core/Managers/Game.h"

// Forward declerations
class Object;
class CustomerBrain;

class Customer : public Component
{
public:
	// Constructors
	Customer(Game& game, Object& object);
	virtual ~Customer();

	// Setters
	void setCharacter(std::weak_ptr<CharacterData> character);
	void setCustomer(CustomerTrait trait, uint16_t funds);

	// Functions
	void reactToPriceOffered(Item* item);

	void enter();
	void leave();
	void update(float delta_time) override;

private:
	// Constant
	const uint8_t drop_range = 70u;

	// Variables
	std::weak_ptr<CharacterData> character;
	CustomerAnimator* animator;
	Object* storage;
	Object* receive_region;

	CustomerTrait trait = CustomerTrait::OpenMinded;
	uint16_t funds = 0u;
	bool stated_request = false;

	// Functions
	CustomerRequest generateRequest();
	void handleRequest(CustomerRequest request);
	void generateSellOffer();
	void determineBuyOffer();

	void handleAcceptableOffer();
	void handleUnacceptableOffer();

	void negotiate(uint16_t new_offer);
};

