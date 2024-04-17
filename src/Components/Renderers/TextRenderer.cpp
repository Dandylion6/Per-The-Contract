#include <iosfwd>
#include <sstream>
#include <string>
#include <vector>

#include "Core/Utility/Vector2.h"
#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/Rect.hpp"

#include "Components/Renderers/Renderer.h"
#include "Components/Renderers/TextRenderer.h"
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
// Getters

Vector2 TextRenderer::getSize() const {
	return text.getLocalBounds().getSize();
}

std::string TextRenderer::getText() const {
	return text.getString();
}


//____________
// Setters

void TextRenderer::setText(std::string text) {
	this->text.setString(text);
	if (max_width > 0.f) wrapText(); // Wrap if a max width is given
}

void TextRenderer::setColor(sf::Color color) {
	this->text.setFillColor(color);
}

void TextRenderer::setMaxWidth(float max_width) {
	this->max_width = max_width;
	wrapText();
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
	// No need to wrap if less than max width
	if (text.getLocalBounds().getSize().x <= max_width) return;

	std::string str = text.getString(); // Get the original text string
	std::string word;
	std::vector<std::string> words;
	std::istringstream iss(str);

	while (iss >> word) {
		words.push_back(word); // Split the original text into individual words
	}

	std::string wrappedText;

	for (const std::string& w : words) {
		// Set the text of partial text string
		text.setString(wrappedText + w);

		// Get the bounds of the partial text
		sf::FloatRect bounds = text.getLocalBounds();

		// Add new line if bounds is over max width
		if (bounds.width < max_width) wrappedText += w + " ";
		else wrappedText += "\n" + w + " ";
	}

	text.setString(wrappedText);
}
