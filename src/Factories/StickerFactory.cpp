#include <cstdint>
#include <string>

#include <SFML/Graphics/Color.hpp>

#include "Components/Renderers/SpriteRenderer.h"
#include "Components/Renderers/TextRenderer.h"
#include "Components/Sticker/Sticker.h"
#include "Core/Component.h"
#include "Core/Object.h"
#include "Factories/StickerFactory.h"


StickerFactory* StickerFactory::instance = nullptr;

//_______________
// Constructors

StickerFactory::StickerFactory(Game& game) : game(game) {
	if (instance != nullptr) {
		delete this;
		return;
	}
	instance = this;
}

StickerFactory::~StickerFactory() {
}


//__________
// Getters

StickerFactory& StickerFactory::getInstance() {
	return *instance;
}


//___________________
// Public functions

Sticker* StickerFactory::createSticker(uint16_t current_price) const {
	std::string name = "sticker_" + std::to_string(current_price);
	Object* sticker_object = new Object(game, name);

	// Sticket length based on character length
	std::string sprite;
	if (current_price < 10u) sprite = short_path;
	else if (current_price < 100u) sprite = middle_path;
	else sprite = long_path;

	SpriteRenderer* sprite_renderer = new SpriteRenderer(
		game, *sticker_object, sprite
	);
	TextRenderer* text_renderer = new TextRenderer(
		game, *sticker_object, std::to_string(current_price)
	);
	text_renderer->setColor(sf::Color(36u, 34u, 46u));
	Collider* collider = new Collider(
		game, *sticker_object, sprite_renderer->getSize()
	);
	return new Sticker(game, *sticker_object, *collider, current_price);
}