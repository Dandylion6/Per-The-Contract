#include "Components/Objects/ShopDoor.h"
#include "Core/Component.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Core/Utility/Math.h"
#include "Core/Utility/Vector2.h"


//______________
// Construtors

ShopDoor::ShopDoor(Game& game, Object& object)
	: Component(game, object) 
{
	object.setZIndex(3);
	object.setAnchor(Vector2(0.f, 0.f));
	object.setPosition(opened_position);
}

ShopDoor::~ShopDoor() {
}

bool ShopDoor::isMoving() const {
    return this->is_closing || this->is_opening;
}


//__________________
// Public function

void ShopDoor::closeDoor() {
    animation_time = 0.f;
    is_closing = true;
}

void ShopDoor::openDoor() {
    animation_time = 0.f;
    is_opening = true;
}

void ShopDoor::update(float delta_time) {
    if (is_closing) {
        animation_time = animation_time + delta_time;
        float time = utils::clamp(animation_time + delta_time, 0.f, close_time);

        object.setPosition(animationFunction(time / close_time));
        if (animation_time >= close_time + extra_closed_wait) is_closing = false;
    } else if (is_opening) {
        animation_time = utils::clamp(animation_time + delta_time, 0.f, close_time);
        object.setPosition(animationFunction(animation_time / close_time));
        if (animation_time >= close_time) is_opening = false;
    }
}

Vector2 ShopDoor::animationFunction(float time) {
    float value = 0.f;

    const float n1 = 7.5625f;
    const float d1 = 2.75f;

    if (time < 1.0f / d1) {
        value = n1 * time * time;
    } else if (time < 2.0f / d1) {
        time -= 1.5f / d1;
        value = n1 * time * time + 0.75f;
    } else if (time < 2.5f / d1) {
        time -= 2.25f / d1;
        value = n1 * time * time + 0.9375f;
    } else {
        time -= 2.625f / d1;
        value = n1 * time * time + 0.984375f;
    }

    if (is_closing) {
        return Vector2::lerp(opened_position, closed_position, utils::clamp01(value));
    } else {
        return Vector2::lerp(closed_position, opened_position, utils::clamp01(value));
    }
}
