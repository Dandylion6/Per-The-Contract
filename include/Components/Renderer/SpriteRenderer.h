#pragma once

#include <memory>
#include <string>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "Core/Component.h"
#include "Core/Interfaces/Renderable.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"

class SpriteRenderer : public Component, public Renderable
{
public:
	// Constructors
	SpriteRenderer(Game& game, Object& object, std::string path);
	virtual ~SpriteRenderer();

	// Functions
	void newSprite(std::string path);
	void render(sf::RenderTarget& target) override;
	void update(float delta_time) override;

private:
	// Variables
	std::unique_ptr<sf::Sprite> sprite = nullptr;
	std::unique_ptr<sf::Texture> texture = nullptr;
};