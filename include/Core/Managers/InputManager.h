#pragma once

#include "SFML/Graphics/RenderWindow.hpp"

class InputManager
{
public:
	// Constructors
	InputManager(sf::RenderWindow& window);
	virtual ~InputManager();

	// Functions
	void pollEvents();

private:
	// References
	sf::RenderWindow& window;
};