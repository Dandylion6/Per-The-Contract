#include <string>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>

#include "Components/Renderers/Renderer.h"
#include "Components/Renderers/TextRenderer.h"
#include "Core/Component.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"


//_______________
// Constructors

TextRenderer::TextRenderer(
	Game& game, Object& object
) : Renderer(game, object) {
	font.loadFromFile(font_path);
	text.setFont(font);
	font.setSmooth(false); // Keep text crisp
}

TextRenderer::TextRenderer(
	Game& game, Object& object, std::string text
) : TextRenderer(game, object) {
	setText(text);
}


//____________
// Accessors

void TextRenderer::setText(std::string text) {
	this->text.setString(text);
}

void TextRenderer::setColor(sf::Color color) {
	this->text.setFillColor(color);
}


//___________________
// Public functions

void TextRenderer::update(float delta_time) {
	Vector2 size = text.getLocalBounds().getSize();
	Vector2 origin = size * object.getAnchor();

	text.setOrigin(origin);
	text.setScale(object.getScale());
	text.setPosition(object.getPosition());
	target.draw(text);
}
