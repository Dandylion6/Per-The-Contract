#pragma once

#include "Core/Managers/Game.h"

// Forward declerations
class Object;

class Component
{
public:
	// Constructors
	Component(Game& game, Object& object);
	virtual ~Component();

	// Functions
	virtual void update(float delta_time) = 0;

protected:
	// References
	Game& game;
	Object& object;
};

