#pragma once

#include <list>

#include <SFML/Graphics/RenderWindow.hpp>

#include "Core/Interfaces/Renderable.h"

class Renderer
{
public:
	// Constructors
	Renderer(sf::RenderWindow& window);
	virtual ~Renderer();

	// Functions
	void addRenderable(Renderable* renderable);
	void render();

private:
	// References
	sf::RenderWindow& window;

	// Variables
	std::list<Renderable*> to_render;
};