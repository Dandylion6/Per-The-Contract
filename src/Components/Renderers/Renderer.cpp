#include "Components/Renderers/Renderer.h"
#include "Core/Component.h"
#include "Core/Managers/Game.h"


//_______________
// Constructors

Renderer::Renderer(Game& game, Object& object) 
	: Component(game, object),target(game.getWindow()),
	render_manager(game.getRenderManager())
{
	render_manager.addRenderer(this);
}

Renderer::~Renderer() {
	render_manager.removeRenderer(this);
}


//____________
// Accessors

int Renderer::getZIndex() const {
	return this->z_index;
}

void Renderer::setZIndex(int z_index) {
	this->z_index = z_index;
	pushToFront(); // Sorts the renderer based on z index
}


//___________________
// Public functions

void Renderer::pushToFront() {
	render_manager.pushToFront(*this);
}
