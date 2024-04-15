#include "Factories/DealClosureFactory.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Components/Renderers/SpriteRenderer.h"


//_______________
// Constructors

DealClosureFactory::DealClosureFactory(Game& game) {
	createClosureObjects(game);
}

DealClosureFactory::~DealClosureFactory() {
}


//____________________
// Private functions

void DealClosureFactory::createClosureObjects(Game& game) const {
	Object* panel_object = new Object(game, "closure_panel");
	panel_object->setZIndex(-1);
	panel_object->setPosition(panel_position);
	new SpriteRenderer(game, *panel_object, panel_path);

	Object* accept_button_object = new Object(game, "accept_button", panel_object);
	Object* decline_button_object = new Object(game, "decline_button", panel_object);
	accept_button_object->setLocalPosition(accept_button_position);
	decline_button_object->setLocalPosition(decline_button_position);

	new SpriteRenderer(game, *accept_button_object, accept_button_path);
	new SpriteRenderer(game, *decline_button_object, decline_button_path);
}
