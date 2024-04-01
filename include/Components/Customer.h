#pragma once

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
class Item;

enum class CustomerTrait
{
	Assertive,
	OpenMinded,
	Knowledgeable,
	Frugal,
	Impulsive,
	Trusting
};

class Customer : public Component
{
public:
	// Constructors
	Customer(Game& game, Object& object);
	virtual ~Customer();

	// Setters
	void setCharacter(std::weak_ptr<CharacterData> character);
	void setCustomer(
		CustomerTrait trait,
		uint16_t funds,
		float willingness_factor
	);

	// Functions
	void reactToPriceOffered(Item* item);
	void enter();
	void leave();
	void update(float delta_time) override;

private:
	// Constant
	const uint8_t drop_range = 70u;
	const std::unordered_map<CustomerTrait, float> negotiability_trait{
		{ CustomerTrait::Assertive, 0.2f },
		{ CustomerTrait::OpenMinded, 0.65f },
		{ CustomerTrait::Knowledgeable, 0.45f },
		{ CustomerTrait::Frugal,  0.8f },
		{ CustomerTrait::Impulsive, 0.3f },
		{ CustomerTrait::Trusting, 0.35f }
	};
	const std::unordered_map<CustomerTrait, float> acceptable_range_trait{
		{ CustomerTrait::Assertive, 0.15f },
		{ CustomerTrait::OpenMinded, 0.4f },
		{ CustomerTrait::Knowledgeable, 0.25f },
		{ CustomerTrait::Frugal, 0.2f },
		{ CustomerTrait::Impulsive, 0.55f },
		{ CustomerTrait::Trusting, 0.45f }
	};

	// References
	DialogueManager& dialogue_manager;

	// Variables
	std::weak_ptr<CharacterData> character;
	CustomerAnimator* animator;
	Object* receive_region;

	CustomerTrait trait = CustomerTrait::OpenMinded;
	uint16_t funds = 0u;
	uint16_t perceived_item_value = 0u;
	uint16_t acceptable_price = 0u;
	float negotiability_factor = 0.f;
	float acceptable_range_factor = 0.f;
	float willingness_factor = 0.f;

	// Functions
	void generateRequest();
	Item* generateItem();
	void placeSellOffer(Item* to_sell);
	void placeNewPriceOffer(Item* item);
	bool isAcceptablePrice(uint16_t offered_price) const;

	bool willAcceptDeal() const;
	bool willNegotiate() const;
};

