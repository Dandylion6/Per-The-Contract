#pragma once

#include <list>
#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>

// Forward declerations
class Renderer;

class RenderManager
{
public:
	// Constructors
	RenderManager(sf::RenderWindow& window);
	virtual ~RenderManager();

	// Getters
	std::weak_ptr<std::list<Renderer*>> getRenderOrder();

	// Functions
	void addRenderer(Renderer* renderer);
	void removeRenderer(Renderer* renderer);
	void pushToFront(Renderer& renderer);
	void render();

private:
	// References
	sf::RenderWindow& window;

	// Variables
	std::shared_ptr<std::list<Renderer*>> to_render;

	// Function
	static bool compareZIndex(const Renderer* a, const Renderer* b);
};