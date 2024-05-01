#include <cstdint>
#include <iosfwd>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "Core/Utility/Vector2.h"
#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

#include "Components/Renderers/Renderer.h"
#include "Components/Renderers/TextRenderer.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Data/FontStyle.h"


//_______________
// Constructors

TextRenderer::TextRenderer(
	Game& game, Object& object, FontStyle style
) : 
	Renderer(game, object) 
{
	font.loadFromFile(font_map.find(style)->second);
	font.setSmooth(true);
	text.setFont(font);
}

TextRenderer::TextRenderer(
	Game& game, Object& object, FontStyle style, std::string text
) : TextRenderer(game, object, style) {
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

bool TextRenderer::isTyping() const {
	return this->text_to_type != nullptr;
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

void TextRenderer::setSize(uint16_t size) {
	text.setCharacterSize(size);
}


//___________________
// Public functions

void TextRenderer::update(float delta_time) {
	if (text_to_type != nullptr) {
		typing_interval_time += delta_time;
		float interval = 1.f / text_per_second;
		if (typing_interval_time >= interval) {
			size_t index = text.getString().getSize() + 1u;
			text.setString((*text_to_type).substr(0u, index));
			if (index >= (*text_to_type).size()) {
				text_to_type.release();
				typing_interval_time = 0.f;
			} else {
				typing_interval_time -= interval;
			}
		}
	}
	updateRenderer();
}

void TextRenderer::addBackground(sf::Color colour, Vector2 border) {
	background = std::make_unique<sf::RectangleShape>();
	background->setSize(Vector2(text.getLocalBounds().getSize()) + border);
	background->setFillColor(colour);
}

void TextRenderer::typeText(std::string to_type) {
	text.setString(to_type);
	if (max_width > 0u) wrapText();
	text_to_type = std::make_unique<std::string>(text.getString());
	text.setString(std::string());
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

	for (const std::string& word : words) {
		// Set the text of partial text string
		text.setString(wrappedText + word + " ");

		// Get the bounds of the partial text
		sf::FloatRect bounds = text.getLocalBounds();

		// Add new line if bounds is over max width
		if (bounds.width < max_width) wrappedText += word + " ";
		else wrappedText += "\n" + word + " ";
	}

	text.setString(wrappedText);
}

void TextRenderer::updateRenderer() {
	const sf::FloatRect rect = text.getLocalBounds();
	Vector2 originOffset = Vector2(rect.getSize()) * object.getAnchor();
	Vector2 origin = Vector2(rect.left + originOffset.x, rect.top + originOffset.y);

	text.setOrigin(origin);
	text.setScale(object.getScale());
	text.setPosition(object.getPosition());
	text.setRotation(object.getRotation());

	if (background != nullptr) {
		origin = Vector2(background->getSize()) * object.getAnchor();
		background->setOrigin(origin);
		background->setScale(object.getScale());
		background->setPosition(object.getPosition());
		background->setRotation(object.getRotation());
		target.draw(*background);
	}

	target.draw(text);
}
