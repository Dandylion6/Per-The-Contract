#pragma once

#include "Core/Component.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"

class Clock : public Component {
public:
	// Constructors
	Clock(Game& game, Object& object);
	virtual ~Clock();

	// Functions
	virtual void update(float delta_time) override;
};

