#include <memory>

#include "Components/Customer.h"
#include "Components/Renderers/SpriteRenderer.h"
#include "Core/Component.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Core/Utility/Math.h"


//_______________
// Constructors

Customer::Customer(
	Game& game, 
	Object& object
) : Component(game, object) {
	torso_object = object.getChild("torso");
	head_object = object.getChild("head");
}

Customer::~Customer() {
}


//__________
// Setters

void Customer::setCharacter(std::weak_ptr<CharacterData> character) {
	this->character = character;
}

//___________________
// Public functions

void Customer::enter() {
	play_time = 0.f;
	is_leaving = false; // To make sure don't both happen
	is_entering = true;
}

void Customer::leave() {
	play_time = 0.f;
	is_entering = false; // To make sure don't both happen
	is_ready = false;
	is_leaving = true;
}

void Customer::update(float delta_time) {
	if (is_entering) enterAnimation(delta_time);
	if (is_leaving) leaveAnimation(delta_time);
	if (!is_ready) return; // Don't continue if not ready for interaction
	idleAnimation(delta_time);
}


//____________________
// Private functions

void Customer::enterAnimation(float delta_time) {
	play_time = utils::clamp(play_time + delta_time, 0.f, enter_time);

	float walk_factor = 1.f;
	if (play_time >= walk_stop_time) { // Lerp walk to stop
		walk_factor = (play_time - walk_stop_time) / walk_stop_period;
		walk_factor = utils::outQuad(1.f, 0.f, walk_factor);
	}

	enterWalkMotion(delta_time, walk_factor);

	// Scale customer entering
	float time = play_time / enter_time;
	float scale = utils::outQuad(enter_begin_scale, 1.f, time);
	torso_object->setScale(Vector2::scale(scale));
	head_object->setScale(Vector2::scale(scale));

	// End animation
	if (play_time == enter_time) {
		play_time = 0.f;
		is_entering = false;
		is_ready = true;
	}
}

void Customer::enterWalkMotion(float delta_time, float walk_factor) {
	float torso_t = walk_speed * play_time;
	float head_t = walk_speed * (play_time - walk_head_offset);

	Vector2 torso_offset = Vector2(
		cosf(torso_t) * walk_sway,
		std::fabsf(sinf(torso_t)) * -walk_height
	);

	Vector2 head_offset = Vector2(
		cosf(head_t) * walk_sway,
		std::fabsf(sinf(head_t)) * -walk_height
	);

	torso_object->setLocalPosition(torso_offset * walk_factor);
	head_object->setLocalPosition(head_offset * walk_factor);
}

void Customer::idleAnimation(float delta_time) {
	play_time += delta_time;
	float head_phase = play_time - idle_head_offset;
	float motion_lerp = 1.f;

	if (play_time < idle_transition_time) {
		motion_lerp = play_time / idle_transition_time;
	}

	Vector2 torso_offset = Vector2(
		sinf(idle_motion_speed * play_time),
		sinf(2.f * idle_motion_speed * play_time)
	) * motion_lerp * 0.8f * idle_motion_strength;

	Vector2 head_offset = Vector2(
		sinf(idle_motion_speed * head_phase),
		sinf(2.f * idle_motion_speed * head_phase)
	) * motion_lerp * idle_motion_strength;

	torso_object->setLocalPosition(torso_offset);
	head_object->setLocalPosition(head_offset);
}

void Customer::leaveAnimation(float delta_time) {
	play_time += delta_time;
	if (play_time > leave_time) {
		is_entering = false;
		play_time = leave_time;
	}
}
