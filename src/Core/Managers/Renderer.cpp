#include <list>
#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>

#include "Core/Interfaces/Renderable.h"
#include "Core/Managers/Game.h"
#include "Core/Managers/Renderer.h"


//_______________
// Constructors

Renderer::Renderer(sf::RenderWindow& window) : window(window) {
}

Renderer::~Renderer() {
}


//___________________
// Public functions

void Renderer::addRenderable(std::weak_ptr<Renderable> renderable) {
	to_render.push_back(renderable);
}

void Renderer::render() {
	for (auto it = to_render.begin(); it != to_render.end();) {
		std::weak_ptr<Renderable> renderable = *it;
		std::shared_ptr<Renderable> target = renderable.lock();

		// Remove if null
		if (target == nullptr) {
			to_render.erase(it);
			continue;
		}

		// Call render
		target->render(window);
		++it;
	}
}
