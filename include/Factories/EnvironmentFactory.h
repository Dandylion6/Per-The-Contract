#pragma once
#include <string>

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

	// Getters 
	Vector2 getDialogueBoxSize() const;
	Vector2 getItemRegionSize() const;

	// Functions
	void createOutside();
	void createDialogue();
	void createStorage();
	void createCounter();
	void createItemRegions(Object* counter);

private:
	// Constants
	const std::string layout_path = "assets/sprites/layout/";

	// References
	Game& game;

	// Variables
	Vector2 dialogue_box_size;
	Vector2 item_region_size;
};

