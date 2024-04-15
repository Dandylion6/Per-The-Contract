#pragma once

#include "Components/Buttons/Button.h"
#include "Components/Collider.h"

// Forward declerations
class Game;
class Object;

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
