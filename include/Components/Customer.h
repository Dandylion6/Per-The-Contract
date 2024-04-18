#pragma once

#include <cstdint>

#include "Components/CustomerAnimator.h"
#include "Core/Component.h"
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
	void dropCash(uint16_t value);

	void enter();
	void leave();
	void update(float delta_time) override;

private:
	// Constant
	const float drop_radius = 60.f;

	// Variables
	CustomerAnimator* animator;
	Object* storage;
	Object* receive_region;

	// Functions
	void handleRequest();
	void placeNewContract();
	void placeSellItem();
	void negotiate(uint16_t new_offer);
};

