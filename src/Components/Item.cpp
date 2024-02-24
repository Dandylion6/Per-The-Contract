#include "Components/Collider.h"
#include "Components/Item.h"
#include "Components/Renderers/Renderer.h"
#include "Core/Component.h"
#include "Core/Object.h"
#include "Data/ItemData.h"


//_______________
// Constructors

Item::Item(
	Game& game,
	Object& object,
	Renderer& renderer,
	Collider& collider,
	ItemData data
) : 
	Drag(game, object, renderer, collider),
	data(data) 
{
	last_dropped = collider.getMostOverlapping(Layer::ItemDrop);
	receive_region = Collider::getColliderWithLayer(Layer::ItemReceive);
}

Item::~Item() {
}


//__________
// Setters

void Item::setOwned(bool owned_by_player) {
	this->owned_by_player = owned_by_player;
}


//___________________
// Public functions

void Item::drag(Vector2& mouse_position, float delta_time) {
	if (owned_by_player) {
		Drag::drag(mouse_position, delta_time);
		return;
	}
	// Confine item to receive region when not baught yet
	object.setPosition(mouse_position + grab_offset);
	collider.fitInto(receive_region);
}

void Item::drop(Vector2& mouse_position) {
	Drag::drop(mouse_position); // Call base function
	if (!owned_by_player) return; // Return because no need for check

	Collider* fit_to = nullptr;
	fit_to = collider.getMostOverlapping(Layer::ItemDrop);

	// fit to last collider if not overlapping
	last_dropped = fit_to == nullptr ? last_dropped : fit_to;
	collider.fitInto(last_dropped);
	object.setParent(&last_dropped->getObject()); // Set region as parent
}
