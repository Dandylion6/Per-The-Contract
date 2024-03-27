#include "Components/Collider.h"
#include "Components/Item.h"
#include "Core/Component.h"
#include "Core/Object.h"
#include "Data/ItemData.h"


//_______________
// Constructors

#include <iostream>

Item::Item(
	Game& game,
	Object& object,
	Collider& collider,
	ItemData& data
) : 
	Drag(game, object, collider),
	data(data) 
{
	last_dropped = collider.getMostOverlapping(Layer::ItemDrop);
	receive_region = Collider::getColliderWithLayer(Layer::ItemReceive);
	if (last_dropped == nullptr) last_dropped = receive_region;
	collider.fitInto(last_dropped);
}

Item::~Item() {
}


//__________
// Getters

ItemData& Item::getData() const {
	return this->data;
}

bool Item::getOwnedByPlayer() const {
	return this->owned_by_player;
}

uint16_t Item::getPrice() const {
	return this->price;
}


//__________
// Setters

void Item::setOwned(bool owned_by_player) {
	this->owned_by_player = owned_by_player;
}

void Item::setPrice(uint16_t price) {
	this->price = price;
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
	Drag::drop(mouse_position);
	if (!owned_by_player) return; // Return because no need for check


	// fit to last collider if not overlapping
	Collider* fit_to = collider.getMostOverlapping(Layer::ItemDrop);
	last_dropped = fit_to == nullptr ? last_dropped : fit_to;
	collider.fitInto(last_dropped);
	object.setParent(&last_dropped->getObject()); // Set region as parent
}
