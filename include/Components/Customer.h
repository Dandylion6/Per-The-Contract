#pragma once

#include "Components/CustomerAnimator.h"
#include "Components/Renderers/SpriteRenderer.h"
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
	void setCharacter(std::weak_ptr<CharacterData> character);

	// Functions
	void enter();
	void leave();
	void update(float delta_time) override;

private:
	// Variables
	std::weak_ptr<CharacterData> character;
	CustomerAnimator* animator;
};

