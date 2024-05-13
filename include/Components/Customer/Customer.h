#pragma once

#include <cstdint>
#include <unordered_map>

#include "Brains/CustomerBrain.h"
#include "Components/Customer/Brains/FirmBrain.h"
#include "Components/Customer/Brains/HagglerBrain.h"
#include "Components/Customer/CustomerAnimator.h"
#include "Components/Objects/Contract.h"
#include "Core/Component.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Data/CustomerTrait.h"

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
	const std::unordered_map<CustomerTrait, CustomerBrain*> brain_map{
		{ CustomerTrait::Firm, new FirmBrain(game) },
		{ CustomerTrait::Haggler, new HagglerBrain(game) }
	};

	// Variables
	CustomerBrain* brain = nullptr;
	CustomerAnimator* animator = nullptr;
	Object* storage = nullptr;
	Object* receive_region = nullptr;
	bool is_thinking = false;
	float thinking_time = 0.f;

	// Functions
	void handleRequest();
	Contract* placeNewContract();
	void placeSellItem();
};

