#pragma once

#include <string>

#include <SFML/Graphics/RenderTarget.hpp>

#include "Core/Component.h"

// Forward declerations
class Game;
class RenderManager;
class Object;

class Renderer : public Component
{
public:
	// Constructors
	Renderer(Game& game, Object& object);
	virtual ~Renderer();

	// Accessors
	int getZIndex() const;
	void setZIndex(int z_index);

	// Functions
	void pushToFront();
	virtual void update(float delta_time) = 0;
	virtual void render() = 0;

protected:
	// References
	sf::RenderTarget& target;
	RenderManager& render_manager;

	// Variables
	int z_index = 0;
};

