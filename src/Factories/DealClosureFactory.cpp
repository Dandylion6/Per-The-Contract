#include "Factories/DealClosureFactory.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Components/Renderers/SpriteRenderer.h"
#include "Components/Buttons/AcceptDealButton.h"
#include "Components/Buttons/DeclineDealButton.h"


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

	Object* accept_object = new Object(game, "accept_button", panel_object);
	Object* decline_object = new Object(game, "decline_button", panel_object);
	accept_object->setLocalPosition(accept_button_position);
	decline_object->setLocalPosition(decline_button_position);

	SpriteRenderer* accept_renderer = new SpriteRenderer(game, *accept_object, accept_button_path);
	SpriteRenderer* decline_renderer = new SpriteRenderer(game, *decline_object, decline_button_path);

	Collider* accept_collider = new Collider(game, *accept_object, accept_renderer->getSize());
	Collider* decline_collider = new Collider(game, *decline_object, decline_renderer->getSize());

	new AcceptDealButton(game, *accept_object, *accept_collider);
	new DeclineDealButton(game, *decline_object, *decline_collider);
}
