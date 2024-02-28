#include "Components/CustomerAnimator.h"
#include "Core/Component.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Core/Utility/Math.h"
#include "Core/Utility/Vector2.h"


//_______________
// Constructors

CustomerAnimator::CustomerAnimator(
	Game& game, Object& object
) : Component(game, object) {
	torso_object = object.getChild("torso");
	head_object = object.getChild("head");
}

CustomerAnimator::~CustomerAnimator() {
}


//____________
// Accessors

CustomerAnimState CustomerAnimator::getAnimationState() const {
	return this->animation_state;
}

void CustomerAnimator::setAnimation(CustomerAnimState state) {
	this->animation_state = state;
	play_time = 0.f;
}


//____________________
// Public functions

void CustomerAnimator::update(float delta_time) {
	switch (animation_state) {
		case None: break;
		case Entering:
		{
			enterAnimation(delta_time);
			break;
		}
		case Idling:
		{
			idleAnimation(delta_time);
			break;
		}
		case Leaving:
		{
			leaveAnimation(delta_time);
			break;
		}
	}
}


//____________________
// Private functions

// Not that pretty code to look at so I just collapse them...

void CustomerAnimator::enterAnimation(float delta_time) {
	play_time = utils::clamp(play_time + delta_time, 0.f, enter_time);

	float walk_factor = 1.f;
	if (play_time >= walk_stop_time) { // Lerp walk to stop
		walk_factor = (play_time - walk_stop_time) / walk_stop_period;
		walk_factor = utils::outQuad(1.f, 0.f, walk_factor);
	}

	enterWalkingMotion(delta_time, walk_factor);

	// Scale customer entering
	float time = play_time / enter_time;
	float scale = utils::outQuad(enter_begin_scale, 1.f, time);
	torso_object->setScale(Vector2::scale(scale));
	head_object->setScale(Vector2::scale(scale));

	// At animation end
	if (play_time == enter_time) {
		animation_state = CustomerAnimState::Idling;
		play_time = 0.f;
	}
}

void CustomerAnimator::enterWalkingMotion(
	float delta_time, float walk_factor
) {
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

void CustomerAnimator::idleAnimation(float delta_time) {
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

void CustomerAnimator::leaveAnimation(float delta_time) {
	// At animation end
	if (play_time == leave_time) {
		animation_state = CustomerAnimState::None;
		play_time = 0.f;
	}
}
