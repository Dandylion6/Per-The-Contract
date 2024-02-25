#include <memory>
#include <string>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "Components/Renderers/Renderer.h"
#include "Components/Renderers/SpriteRenderer.h"
#include "Core/Component.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Core/Utility/Vector2.h"


//_______________
// Constructors

SpriteRenderer::SpriteRenderer(Game& game, Object& object)
	: Renderer(game, object) 
{
	// Create empty texture and sprite
	texture = std::make_unique<sf::Texture>();
	sprite = std::make_unique<sf::Sprite>();
}

SpriteRenderer::SpriteRenderer(
	Game& game, Object& object, std::string path
) : SpriteRenderer(game, object) {
	setSprite(path);
}

SpriteRenderer::~SpriteRenderer() {
}

Vector2 SpriteRenderer::getSize() const {
	return texture->getSize();
}

void SpriteRenderer::setSprite(std::string path) {
	if (path.empty()) {
		path = "assets/sprites/placeholder.png";
	} // Use placeholder is empty string
	if (texture->loadFromFile(path)) {
		sprite->setTexture(*texture, true);
	}
}


//___________________
// Public functions

void SpriteRenderer::update(float delta_time) {
	Vector2 origin = Vector2(texture->getSize()) * object.getAnchor();
	sprite->setOrigin(origin);
	sprite->setScale(object.getScale());
	sprite->setPosition(object.getPosition());
	target.draw(*sprite);
}
