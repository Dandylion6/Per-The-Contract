#include "Components/Objects/Clock.h"
#include "Core/Component.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"


//_______________
// Constructors

Clock::Clock(Game& game, Object& object) : 
	Component(game, object) {
}

Clock::~Clock() {
}


//___________________
// Public functions

void Clock::update(float delta_time) {
	object.setRotation((game.getTimeOfDayFloat() / 12.f) * 360.f);
}

