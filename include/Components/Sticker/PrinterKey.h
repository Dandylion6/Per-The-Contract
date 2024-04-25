#pragma once

#include <cstdint>
#include <map>

#include "SFML/Window/Keyboard.hpp"

#include "Components/Collider.h"
#include "Components/Sticker/StickerPrinter.h"
#include "Core/Component.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"

class PrinterKey : Component
{
public:
	// Constructors
	PrinterKey(
		Game& game, Object& object,
		StickerPrinter& printer, Collider& collider,
		uint8_t output
	);
	virtual ~PrinterKey();

	// Functions
	void update(float delta_time) override;

private:
	// Constants
	std::map<uint8_t, sf::Keyboard::Key> key_map = {
		{0u, sf::Keyboard::Num0}, {1u, sf::Keyboard::Num1},
		{2u, sf::Keyboard::Num2}, {3u, sf::Keyboard::Num3},
		{4u, sf::Keyboard::Num4}, {5u, sf::Keyboard::Num5},
		{6u, sf::Keyboard::Num6}, {7u, sf::Keyboard::Num7},
		{8u, sf::Keyboard::Num8}, {9u, sf::Keyboard::Num9},
		{10u, sf::Keyboard::Enter}, {11u, sf::Keyboard::BackSpace},
	};
	std::map<uint8_t, sf::Keyboard::Key> key_num_map = {
		{0u, sf::Keyboard::Numpad0}, {1u, sf::Keyboard::Numpad1},
		{2u, sf::Keyboard::Numpad2}, {3u, sf::Keyboard::Numpad3},
		{4u, sf::Keyboard::Numpad4}, {5u, sf::Keyboard::Numpad5},
		{6u, sf::Keyboard::Numpad6}, {7u, sf::Keyboard::Numpad7},
		{8u, sf::Keyboard::Numpad8}, {9u, sf::Keyboard::Numpad9},
		{10u, sf::Keyboard::Enter}, {11u, sf::Keyboard::Subtract}
	};

	// References
	StickerPrinter& printer;
	Collider& collider;

	// Variables
	sf::Keyboard::Key key;
	sf::Keyboard::Key num_key;
	bool key_pressed = false;
	bool mouse_clicked = false;
	bool target_clicked = false;
	uint8_t output;

	// Functions
	void handleKeyInput();
	void handleMouseInput();
	void keyPress();
	void keyRelease();
};

