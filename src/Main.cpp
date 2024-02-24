#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowBase.hpp>
#include <SFML/Window/WindowStyle.hpp>

#include "Core/Managers/Game.h"
#include "Core/Managers/InputManager.h"
#include "Core/Managers/RenderManager.h"

int main() {
	// Create window
	sf::RenderWindow window(
		sf::VideoMode(1920u, 1080u),
		"The Merchant",
		sf::Style::Fullscreen
	);

	// Instanciate core classes
	InputManager input(window);
	RenderManager renderer(window);
	Game game(window, renderer);

	// Delta time clock
	sf::Clock delta_time_clock;

	// Runtime loop
	while (window.isOpen()) {
		// Get delta time
		float delta_time = delta_time_clock.restart().asSeconds();

		// Update
		input.pollEvents();
		game.update(delta_time);
		renderer.render();
	}

	// Exit game
	return 0;
}