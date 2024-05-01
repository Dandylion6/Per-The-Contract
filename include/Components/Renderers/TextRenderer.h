#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <string>

#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Text.hpp"

#include "Components/Renderers/Renderer.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Core/Utility/Vector2.h"
#include "Data/FontStyle.h"

class TextRenderer : public Renderer
{
public:
	// Constructors
	TextRenderer(Game& game, Object& object, FontStyle style);
	TextRenderer(Game& game, Object& object, FontStyle style, std::string text);

	// Getters
	Vector2 getSize() const;
	std::string getText() const;
	bool isTyping() const;

	// Setters
	void setText(std::string to_type);
	void setColor(sf::Color color);
	void setMaxWidth(float max_width);
	void setSize(uint16_t size);

	// Functions
	void typeText(std::string text);
	void addBackground(sf::Color colour, Vector2 border);
	void update(float delta_time) override;

private:
	// Paths
	const std::string font_directory = "assets/fonts/";
	const std::map<FontStyle, std::string> font_map{
		{ FontStyle::Roboto, font_directory + "Roboto.ttf" },
		{ FontStyle::IMFellDWPica, font_directory + "IMFellDWPica.ttf" },
		{ FontStyle::LibreBaskerville, font_directory + "LibreBaskerville.ttf" },
		{ FontStyle::CourierPrime, font_directory + "CourierPrime.ttf" },
		{ FontStyle::Caveat, font_directory + "Caveat.ttf" },
	};
	const float text_per_second = 75.f;

	// Variables
	sf::Font font;
	sf::Text text;
	std::unique_ptr<sf::RectangleShape> background = nullptr;
	std::unique_ptr<std::string> text_to_type = nullptr;
	float max_width = 0.f;
	float typing_interval_time = 0.f;

	// Functions
	void wrapText();
	void updateRenderer();
};