#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

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