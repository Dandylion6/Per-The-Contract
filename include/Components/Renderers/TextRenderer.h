#pragma once

#include <map>
#include <string>

#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Text.hpp"

#include "Components/Renderers/Renderer.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Core/Utility/Vector2.h"

enum class FontStyle {
	Roboto,
	IMFellDWPica,
	LibreBaskerville,
	CourierPrime,
	Caveat
};

class TextRenderer : public Renderer
{
public:
	// Constructors
	TextRenderer(Game& game, Object& object, FontStyle style);
	TextRenderer(Game& game, Object& object, FontStyle style, std::string text);

	// Getters
	Vector2 getSize() const;
	std::string getText() const;

	// Setters
	void setText(std::string text);
	void setColor(sf::Color color);
	void setMaxWidth(float max_width);

	// Functions
	void update(float delta_time) override;

private:
	// Paths
	const std::string font_directory = "assets/fonts/";
	const std::string default_font_path = font_directory + "Roboto.ttf";
	const std::string merchant_font_path = font_directory + "IMFellDWPica.ttf";
	const std::string customer_font_path = font_directory + "LibreBaskerville.ttf";
	const std::string contract_font_path = font_directory + "CourierPrime.ttf";
	const std::string written_font_path = font_directory + "Caveat.ttf";
	const std::map<FontStyle, std::string> font_map{
		{ FontStyle::Roboto, font_directory + "Roboto.ttf" },
		{ FontStyle::IMFellDWPica, font_directory + "IMFellDWPica.ttf" },
		{ FontStyle::LibreBaskerville, font_directory + "LibreBaskerville.ttf" },
		{ FontStyle::CourierPrime, font_directory + "CourierPrime.ttf" },
		{ FontStyle::Caveat, font_directory + "Caveat.ttf" },
	};

	// Variables
	sf::Font font;
	sf::Text text;
	float max_width = 0.f;

	// Functions
	void wrapText();
};