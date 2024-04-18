#pragma once

#include "Components/Buttons/Button.h"
#include "Components/Collider.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"

class DeclineDealButton : Button {
public:
	// Constructors
	DeclineDealButton(Game& game, Object& object, Collider& collider);
	virtual ~DeclineDealButton();

private:
	// Functions
	virtual void buttonPressed() override;
	virtual void buttonReleased() override;
};
