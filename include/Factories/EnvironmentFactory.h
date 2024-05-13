#pragma once
#include <string>

#include "Core/Managers/Game.h"
#include "Core/Object.h"

class EnvironmentFactory
{
public:
	// Constructors
	EnvironmentFactory(Game& game);
	virtual ~EnvironmentFactory();

private:
	// Constants
	const std::string layout_path = "assets/sprites/layout/";
	const std::string clock_path = "assets/sprites/objects/clock/";

	// References
	Game& game;

	// Functions
	void createOutside();
	void createDialogue();
	void createStorage();
	void createCounter();
	void createItemRegions(Object* counter);
};

