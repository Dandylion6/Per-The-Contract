#pragma once

#include <string>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

#include "Components/Renderers/Renderer.h"
#include "Core/Component.h"
#include "Core/Utility/Vector2.h"

// Forward declerations
class Game;
class Object;

class TextRenderer : public Renderer
{
public:
	// Constructors
	TextRenderer(Game& game, Object& object);
	TextRenderer(Game& game, Object& object, std::string text);

	// Getters
	Vector2 getSize() const;

	// Setters
	void setText(std::string text);
	void setColor(sf::Color color);
	void setMaxWidth(float max_width);

	// Functions
	void update(float delta_time) override;

private:
	// Paths
	const std::string font_path = "assets/hypnoverse.ttf";

	// Variables
	sf::Font font;
	sf::Text text;
	float max_width;

	// Functions
	void wrapText();
};