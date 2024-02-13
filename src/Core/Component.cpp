#include "Core/Component.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"


//_______________
// Constructors

Component::Component(
	Game& game, Object& object
) : game(game), object(object) {
}

Component::~Component() {
}
