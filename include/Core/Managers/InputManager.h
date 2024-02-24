#pragma once

#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>

#include "Core/Managers/Game.h"

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