#pragma once

#include <string>
#include <vector>

// Forward decleration
class Game;
class Component;

class Object
{
public:
	// Constructors
	Object(Game& game, std::string name);
	virtual ~Object();

	// Getters
	std::string getName() const;

	// Functions
	void addComponent(Component* component);
	void update(float delta_time);

private:
	// References
	Game& game;

	// Variables
	std::string name;
	std::vector<Component*> components;
};