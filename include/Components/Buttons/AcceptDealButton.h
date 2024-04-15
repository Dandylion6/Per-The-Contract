#pragma once

#include "Components/Buttons/Button.h"
#include "Components/Collider.h"

// Forward declerations
class Game;
class Object;

class AcceptDealButton : Button {
public:
	// Constructors
	AcceptDealButton(Game& game, Object& object, Collider& collider);
	virtual ~AcceptDealButton();

private:
	// Functions
	virtual void buttonPressed() override;
	virtual void buttonReleased() override;
};

