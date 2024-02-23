#include "Core/Interfaces/Renderable.h"
#include "Core/Managers/Renderer.h"


//_______________
// Constructors

Renderable::Renderable(Renderer& renderer) : renderer(renderer) {
	renderer.addRenderable(this);
}

Renderable::~Renderable() {
	renderer.removeRenderable(this);
}


//__________
// Getters

int Renderable::getZIndex() const {
	return this->z_index;
}


//__________
// Setters

void Renderable::setZIndex(int z_index) {
	this->z_index = z_index;
}

void Renderable::pushToFront() {
	renderer.pushToFront(*this);
}
