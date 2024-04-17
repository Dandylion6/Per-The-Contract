#pragma once

#include "SFML/Graphics/RenderTarget.hpp"

#include "Core/Component.h"

// Forward declerations
class Game;
class Object;

class Renderer : public Component
{
public:
	// Constructors
	Renderer(Game& game, Object& object);
	virtual ~Renderer();

	// Functions
	virtual void update(float delta_time) = 0;

protected:
	// References
	sf::RenderTarget& target;
};

