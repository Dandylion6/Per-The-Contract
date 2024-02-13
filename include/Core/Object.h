#pragma once
#include <string>

class Game;

class Object
{
public:
	// Constructors
	Object(Game& game, std::string name);
	virtual ~Object();

	// Getters
	std::string getName() const;

private:
	// References
	Game& game;

	// Variables
	std::string name;
};