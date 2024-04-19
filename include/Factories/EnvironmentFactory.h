#pragma once
#include <string>

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

