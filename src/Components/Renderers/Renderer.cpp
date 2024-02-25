#include "Components/Renderers/Renderer.h"
#include "Core/Component.h"
#include "Core/Managers/Game.h"


//_______________
// Constructors

Renderer::Renderer(
	Game& game, Object& object
) : 
	Component(game, object), 
	target(game.getWindow()) {
}

Renderer::~Renderer() {
}
