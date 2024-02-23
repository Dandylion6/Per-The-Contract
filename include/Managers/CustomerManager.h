#pragma once

#include "Components/Customer.h"
#include "Components/Renderer/SpriteRenderer.h"

// Forward declerations
class Game;
class Object;

class CustomerManager
{
public:
	// Constructors
	CustomerManager(Game& game);
	virtual ~CustomerManager();

	// Functions
	void changeCustomer();

private:
	// References
	Game& game;

	// Variables
	Customer* customer = nullptr;
	SpriteRenderer* head_renderer = nullptr;
	SpriteRenderer* torso_renderer = nullptr;

	// Functions
	void createCustomer();
};

