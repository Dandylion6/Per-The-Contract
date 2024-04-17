#pragma once

#include <string>

#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"

#include "Components/Renderers/Renderer.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Core/Utility/Vector2.h"

class SpriteRenderer : public Renderer
{
public:
	// Constructors
	SpriteRenderer(Game& game, Object& object);
	SpriteRenderer(Game& game, Object& object, std::string path);
	virtual ~SpriteRenderer();

	// Accessors
	Vector2 getSize() const;
	void setSprite(std::string path);
	void setAlpha(float alpha);

	// Functions
	void update(float delta_time) override;

private:
	// Variables
	sf::Sprite sprite;
	sf::Texture texture;
	float alpha = 1.f;
};