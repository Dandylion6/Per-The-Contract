#pragma once

#include "Components/Renderers/TextRenderer.h"
#include "Core/Component.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"

class Dialogue : public Component {
public:
	// Constructors
	Dialogue(Game& game, Object& object, TextRenderer& renderer);
	virtual ~Dialogue();

	// Functions
	void update(float delta_time);

private:
	// References
	TextRenderer& renderer;

	// Variables
	bool continued_dialogue = false;
};

