#include <memory>
#include <string>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "Components/Renderer/SpriteRenderer.h"
#include "Core/Component.h"
#include "Core/Interfaces/Renderable.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Core/Utility/Vector2.h"


//_______________
// Constructors

SpriteRenderer::SpriteRenderer(
	Game& game, Object& object, std::string path
) : Component(game, object), Renderable() {
	// Create empty texture and sprite
	texture = std::make_unique<sf::Texture>();
	sprite = std::make_unique<sf::Sprite>();

	game.getRenderer().addRenderable(this); // Add to be rendered
	newSprite(path);
}

SpriteRenderer::~SpriteRenderer() {
}


//___________________
// Public functions

void SpriteRenderer::newSprite(std::string path) {
	if (texture->loadFromFile(path)) {
		sprite->setTexture(*texture, true);
	}
}

void SpriteRenderer::render(sf::RenderTarget& target) {
	Vector2 origin = Vector2(texture->getSize()) * object.getAnchor();
	sprite->setOrigin(origin);
	sprite->setPosition(object.getPosition());
	target.draw(*sprite);
}

void SpriteRenderer::update(float delta_time) {
}
