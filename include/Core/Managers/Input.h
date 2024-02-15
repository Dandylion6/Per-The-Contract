#pragma once

#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>

#include "Core/Managers/Game.h"

class Input
{
public:
	// Constructors
	Input(sf::RenderWindow& window);
	virtual ~Input();

	// Functions
	void pollEvents();

private:
	// References
	sf::RenderWindow& window;
};