#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Window.hpp>
#include <SFML/Window/WindowBase.hpp>

#include "Core/Managers/Input.h"


//_______________
// Constructors

Input::Input(sf::RenderWindow& window) : window(window) {
}

Input::~Input() {
}


//___________________
// Public functions

void Input::pollEvents() {
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
