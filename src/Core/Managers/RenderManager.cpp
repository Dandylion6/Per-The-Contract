#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>

#include "Components/Renderers/Renderer.h"
#include "Core/Managers/RenderManager.h"


//_______________
// Constructors

RenderManager::RenderManager(sf::RenderWindow& window) : window(window) {
}

RenderManager::~RenderManager() {
}


//__________
// Getters

std::weak_ptr<std::list<Renderer*>> RenderManager::getRenderOrder() {
	return this->to_render;
}


//___________________
// Public functions

void RenderManager::addRenderer(Renderer* renderer) {
	auto it = std::upper_bound(
		to_render.begin(), to_render.end(), renderer, compareZIndex
	); // Find insert point based on z index (inserts to the back)
	to_render.insert(it, renderer);
}

void RenderManager::removeRenderer(Renderer* renderer) {
	to_render.remove(renderer);
}

void RenderManager::pushToFront(Renderer& renderer) {
	// Removing and adding will insert renderable at the back
	removeRenderer(&renderer);
	addRenderer(&renderer);
}

void RenderManager::render() {
	// Clear frame
	window.clear();

	// Draw renderables
	for (Renderer* renderer : to_render) {
		if (renderer == nullptr) continue; // This shouldn't happen
		renderer->render();
	}

	// Swap current with new frame
	window.display();
}


//____________________
// Private functions

bool RenderManager::compareZIndex(
	const Renderer* a, const Renderer* b
) {
	return a->getZIndex() < b->getZIndex();
}
