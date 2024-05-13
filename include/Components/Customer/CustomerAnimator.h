#pragma once

#include "Core/Component.h"

// Forward declerations
class Game;
class Object;

enum CustomerAnimState
{
	None,
	Entering,
	Idling
};

class CustomerAnimator : public Component
{
public:
	// Constructors
	CustomerAnimator(Game& game, Object& object);
	virtual ~CustomerAnimator();

	// Accessors
	CustomerAnimState getAnimationState() const;
	void setAnimation(CustomerAnimState state);

	// Functions
	void update(float delta_time) override;

private:
	// Animation constants
	const float enter_time = 2.f;

	const float walk_stop_time = enter_time * 0.8f;
	const float walk_stop_period = enter_time - walk_stop_time;
	const float enter_begin_scale = 0.8f;

	const float idle_head_offset = 0.08f;
	const float idle_transition_time = 1.5f;
	const float idle_motion_speed = 1.6f;
	const float idle_motion_strength = 7.f;

	const float walk_head_offset = 0.04f;
	const float walk_speed = 6.f;
	const float walk_height = 18.f;
	const float walk_sway = 12.f;

	// Variables
	Object* head_object = nullptr;
	Object* torso_object = nullptr;

	float play_time = 0.f;
	CustomerAnimState animation_state = CustomerAnimState::None;

	// Functions
	void enterAnimation(float delta_time);
	void enterWalkingMotion(float delta_time, float walk_factor);
	void idleAnimation(float delta_time);
};

