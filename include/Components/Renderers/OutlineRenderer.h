#pragma once

#include "SFML/Graphics/RectangleShape.hpp"

#include "Components/Renderers/Renderer.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Core/Utility/Vector2.h"

class OutlineRenderer : public Renderer {
public:
	// Constructors
	OutlineRenderer(Game& game, Object& object, Vector2 size);
	virtual ~OutlineRenderer();

	// Functions
	virtual void update(float delta_time) override;

private:
	// Constants
	float flash_speed = 1.25f;
	float thickness = 3.5f;
	float size_offset = 5.f;

	// Variables
	sf::RectangleShape outline;
	Vector2 size;
	float time = 0.f;
};