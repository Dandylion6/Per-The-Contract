#include "Components/Sticker/Sticker.h"
#include "Core/Component.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"


//_______________
// Constructors

Sticker::Sticker(
	Game& game, Object& object, uint16_t price
) : Component(game, object), price(price) {
}

Sticker::~Sticker() {
}


//___________________
// Public functions

void Sticker::update(float delta_time) {
}
