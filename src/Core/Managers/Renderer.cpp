#include <list>
#include <memory>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Window.hpp>

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

	// Draw renderables
	for (Renderable* renderable : to_render) {
		if (renderable == nullptr) {
			to_render.remove(NULL);
			continue;
		}

		// Call render
		renderable->render(window);
	}

	// Swap current with new frame
	window.display();
}
