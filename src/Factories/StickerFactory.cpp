#include <cstdint>
#include <string>

#include "SFML/Graphics/Color.hpp"

#include "Components/Collider.h"
#include "Components/Renderers/SpriteRenderer.h"
#include "Components/Renderers/TextRenderer.h"
#include "Components/Sticker/Sticker.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Core/Utility/Vector2.h"
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
	Object* display_object = new Object(game, name + "_display", sticker_object);
	display_object->setAnchor(Vector2(0.9f, 0.75f));

	SpriteRenderer* sprite_renderer = new SpriteRenderer(game, *sticker_object, sticker_path);
	TextRenderer* text_renderer = new TextRenderer(
		game, *display_object, FontStyle::Caveat, std::to_string(current_price) + "$"
	);
	text_renderer->setColor(sf::Color(26u, 24u, 36u));
	Collider* collider = new Collider(game, *sticker_object, sprite_renderer->getSize());
	return new Sticker(game, *sticker_object, *collider, current_price);
}