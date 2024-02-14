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
	to_render.push_back(renderable);
}

void Renderer::render() {
	// Clear frame
	window.clear();

	// Remove null pointers
	to_render.erase(
		std::remove_if(to_render.begin(), to_render.end(), 
		[](Renderable* pointer) {
			return pointer == nullptr;
		}), to_render.end()
	);

	// Draw renderables
	for (Renderable* renderable : to_render) {
		// Call render
		renderable->render(window);
	}

	// Swap current with new frame
	window.display();
}
