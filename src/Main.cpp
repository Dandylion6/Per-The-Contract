#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Clock.hpp"
#include "SFML/Window/VideoMode.hpp"
#include "SFML/Window/WindowStyle.hpp"

#include "Core/Managers/Game.h"
#include "Core/Managers/InputManager.h"


int main() {
	// Create window
	sf::RenderWindow window(
		sf::VideoMode(1920u, 1080u),
		"The Merchant",
		sf::Style::Fullscreen
	);

	// Instanciate core classes
	InputManager input(window);
	Game game(window);

	// Delta time clock
	sf::Clock delta_time_clock;

	// Runtime loop
	while (window.isOpen()) {
		// Get delta time
		float delta_time = delta_time_clock.restart().asSeconds();
		window.clear(); // Clear frame

		// Update
		input.pollEvents();
		game.update(delta_time);

		// Swap current with new frame
		window.display();
	}

	// Exit game
	return 0;
}