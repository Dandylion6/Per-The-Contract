#pragma once

#include <list>

#include <SFML/Graphics/RenderWindow.hpp>

// Forward declerations
class Renderable;

class Renderer
{
public:
	// Constructors
	Renderer(sf::RenderWindow& window);
	virtual ~Renderer();

	// Functions
	void addRenderable(Renderable* renderable);
	void removeRenderable(Renderable* renderable);
	void pushToFront(Renderable& renderable);
	void render();

private:
	// References
	sf::RenderWindow& window;

	// Variables
	std::list<Renderable*> to_render;

	// Function
	static bool compareZIndex(const Renderable* a, const Renderable* b);
};