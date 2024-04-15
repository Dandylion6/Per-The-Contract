#pragma once

#include "Core/Component.h"
#include "Components/Collider.h"

// Forward declerations
class Game;
class Object;

class Button : public Component {
public:
	// Constructors
	Button(Game& game, Object& object, Collider& collider);
	virtual ~Button();

	// Functions
	virtual void update(float delta_time) override;

protected:
	// References
	Collider& collider;

	// Variables
	bool mouse_pressed = false;

	// Functions
	virtual void buttonPressed() = 0;
	virtual void buttonReleased() = 0;
};

