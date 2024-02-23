#pragma once

#include "Core/Component.h"
#include "Data/CharacterData.h"

// Forward declerations
class Game;
class Object;

class Customer : public Component
{
public:
	// Constructors
	Customer(Game& game, Object& object);
	virtual ~Customer();

	// Setters
	void setCharacter(CharacterData character);

	// Functions
	void update(float delta_time) override;

private:
	// Variables
	CharacterData character = CharacterData();
};

