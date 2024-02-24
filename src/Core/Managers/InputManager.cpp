#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Window.hpp>
#include <SFML/Window/WindowBase.hpp>

#include "Core/Managers/InputManager.h"


//_______________
// Constructors

InputManager::InputManager(sf::RenderWindow& window) : window(window) {
}

InputManager::~InputManager() {
}


//___________________
// Public functions

void InputManager::pollEvents() {
	sf::Event event;

	while (window.pollEvent(event)) {
		switch (event.type) {

			// Close window on close press
			case sf::Event::Closed:
			{
				window.close();
				break;
			}

			// Close window on key press
			case sf::Event::KeyPressed:
			{
				if (event.key.code == sf::Keyboard::F1) {
					window.close();
					break;
				}
			}

			default: break;
		}
	}
}
