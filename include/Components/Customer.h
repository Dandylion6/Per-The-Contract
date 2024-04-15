#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Components/CustomerAnimator.h"
#include "Components/Item.h"
#include "Core/Component.h"
#include "Core/Utility/Vector2.h"
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

	// Functions
	void actOnPlayerOffer();

	void enter();
	void leave();
	void update(float delta_time) override;

private:
	// Constant
	const float drop_radius = 70.f;

	// Variables
	CustomerAnimator* animator;
	Object* storage;
	Object* receive_region;

	bool stated_request = false;

	// Functions
	void handleRequest();
	void placeSellItem();
	void negotiate(uint16_t new_offer);
};

