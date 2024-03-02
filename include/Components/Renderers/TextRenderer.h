#pragma once

#include <string>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

#include "Components/Renderers/Renderer.h"
#include "Core/Component.h"

// Forward declerations
class Game;
class Object;

class TextRenderer : public Renderer
{
public:
	// Constructors
	TextRenderer(Game& game, Object& object);
	TextRenderer(Game& game, Object& object, std::string text);

	// Accessors
	void setText(std::string text);
	void setColor(sf::Color color);

	// Functions
	void update(float delta_time) override;

private:
	// Paths
	const std::string font_path = "assets/hypnoverse.ttf";

	// Variables
	sf::Font font;
	sf::Text text;
};