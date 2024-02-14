#pragma once
#include <string>

// Forward declerations
class Game;

class EnvironmentFactory
{
public:
	// Constructors
	EnvironmentFactory(Game& game);
	virtual ~EnvironmentFactory();

	// Functions
	void createOutside();
	void createDialogue();
	void createStorage();
	void createCounter();
	void createItemRegions();

private:
	// Constants
	const std::string layout_path = "assets/sprites/layout/";

	// References
	Game& game;
};

