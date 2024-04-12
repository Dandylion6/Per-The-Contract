#pragma once
#include <string>

#include "Components/Collider.h"
#include "Core/Utility/Vector2.h"

// Forward declerations
class Game;
class Object;

class EnvironmentFactory
{
public:
	// Constructors
	EnvironmentFactory(Game& game);
	virtual ~EnvironmentFactory();

private:
	// Constants
	const std::string layout_path = "assets/sprites/layout/";

	// References
	Game& game;

	// Functions
	void createOutside();
	void createDialogue();
	void createStorage();
	void createCounter();
	void createItemRegions(Object* counter);
};

