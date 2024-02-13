#pragma once

#include <list>
#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>

#include "Core/Interfaces/Renderable.h"

class Renderer
{
public:
	// Constructors
	Renderer(sf::RenderWindow& window);
	virtual ~Renderer();

	// Functions
	void addRenderable(std::weak_ptr<Renderable> renderable);
	void render();

private:
	// References
	sf::RenderWindow& window;

	// Variables
	std::list<std::weak_ptr<Renderable>> to_render;
};