#pragma once

#include <cmath>

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
	// Animation constants
	const float enter_time = 4.f;
	const float leave_time = 4.f;

	const float walk_stop_time = enter_time * 0.7f;
	const float walk_stop_period = enter_time - walk_stop_time;
	const float enter_begin_scale = 0.75f;
	
	const float idle_head_offset = 0.12f;
	const float idle_transition_time = 1.5f;
	const float idle_motion_speed = 1.6f;
	const float idle_motion_strength = 7.f;

	const float walk_head_offset = 0.08f;
	const float walk_speed = 5.f;
	const float walk_height = 18.f;
	const float walk_sway = 14.f;

	// Variables
	std::weak_ptr<CharacterData> character;
	Object* head_object = nullptr;
	Object* torso_object = nullptr;

	float play_time = 0.f;
	bool is_entering = false;
	bool is_ready = false; // Customer is ready for interactions
	bool is_leaving = false;

	// Functions
	void enterAnimation(float delta_time);
	void enterWalkMotion(float delta_time, float walk_factor);
	void idleAnimation(float delta_time);
	void leaveAnimation(float delta_time);
};

