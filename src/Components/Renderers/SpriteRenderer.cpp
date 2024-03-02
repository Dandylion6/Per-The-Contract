#include <memory>
#include <string>

#include <SFML/Graphics/Color.hpp>
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
}

SpriteRenderer::SpriteRenderer(
	Game& game, Object& object, std::string path
) : SpriteRenderer(game, object) {
	setSprite(path);
}

SpriteRenderer::~SpriteRenderer() {
}

Vector2 SpriteRenderer::getSize() const {
	return texture.getSize();
}

void SpriteRenderer::setSprite(std::string path) {
	if (path.empty()) {
		path = "assets/sprites/placeholder.png";
	} // Use placeholder is empty string
	if (texture.loadFromFile(path)) {
		sprite.setTexture(texture, true);
	}
}

void SpriteRenderer::setAlpha(float alpha) {
	sprite.setColor(sf::Color(255u, 255u, 255u, alpha * 255u));
}


//___________________
// Public functions

void SpriteRenderer::update(float delta_time) {
	Vector2 origin = Vector2(texture.getSize()) * object.getAnchor();
	sprite.setOrigin(origin);
	sprite.setScale(object.getScale());
	sprite.setPosition(object.getPosition());
	target.draw(sprite);
}
