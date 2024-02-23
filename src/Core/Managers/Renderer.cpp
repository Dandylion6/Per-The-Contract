#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>

#include "Core/Interfaces/Renderable.h"
#include "Core/Managers/Renderer.h"


//_______________
// Constructors

Renderer::Renderer(sf::RenderWindow& window) : window(window) {
}

Renderer::~Renderer() {
}


//___________________
// Public functions

void Renderer::addRenderable(Renderable* renderable) {
	auto it = std::upper_bound(
		to_render.begin(), to_render.end(), renderable, compareZIndex
	); // Find insert point based on z index (inserts to the back)
	to_render.insert(it, renderable);
}

void Renderer::removeRenderable(Renderable* renderable) {
	to_render.remove(renderable);
}

void Renderer::pushToFront(Renderable& renderable) {
	// Removing and adding will insert renderable at the back
	removeRenderable(&renderable);
	addRenderable(&renderable); 
}

void Renderer::render() {
	// Clear frame
	window.clear();

	// Draw renderables
	for (Renderable* renderable : to_render) {
		if (renderable == nullptr) continue; // This shouldn't happen
		renderable->render(window);
	}

	// Swap current with new frame
	window.display();
}


//____________________
// Private functions

bool Renderer::compareZIndex(
	const Renderable* a, const Renderable* b
) {
	return a->getZIndex() < b->getZIndex();
}
