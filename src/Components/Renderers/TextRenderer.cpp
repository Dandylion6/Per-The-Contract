#include <sstream>
#include <string>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Rect.hpp>
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

void TextRenderer::setMaxWidth(float max_width) {
	this->max_width = max_width;

	// Wrap text to max width
	const sf::FloatRect textBounds = text.getLocalBounds();
	if (textBounds.width > max_width) {
		wrapText();
	}
}


//___________________
// Public functions

void TextRenderer::update(float delta_time) {
	const sf::FloatRect rect = text.getLocalBounds();
	const Vector2 originOffset = Vector2(rect.getSize()) * object.getAnchor();
	const Vector2 origin = Vector2(
		rect.left + originOffset.x,
		rect.top + originOffset.y
	);

	text.setOrigin(origin);
	text.setScale(object.getScale());
	text.setPosition(object.getPosition());
	text.setRotation(object.getRotation());

	target.draw(text);
}


//___________________
// Private function

void TextRenderer::wrapText() {
	std::string str = text.getString();
	std::string word;
	std::vector<std::string> words;
	std::istringstream iss(str);
	while (iss >> word) {
		words.push_back(word);
	}

	std::string wrappedText;
	float lineWidth = 0.f;
	for (const std::string& w : words) {
		sf::FloatRect bounds = text.getFont()->getGlyph(w[0], text.getCharacterSize(), text.getStyle(), text.getOutlineThickness()).bounds;
		if (lineWidth + bounds.width < max_width) {
			wrappedText += w + " ";
			lineWidth += bounds.width;
		} else {
			wrappedText += "\n" + w + " ";
			lineWidth = bounds.width;
		}
	}

	text.setString(wrappedText);
}
