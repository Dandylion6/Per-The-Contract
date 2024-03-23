#include "Components/Collider.h"
#include "Components/Item.h"
#include "Components/Sticker/Sticker.h"
#include "Core/Component.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"


//_______________
// Constructors

Sticker::Sticker(
	Game& game, Object& object, 
	Collider& collider, uint16_t price
) : 
	Drag(game, object, collider), 
	price(price) {
	last_dropped = collider.getMostOverlapping(Layer::ItemDrop);
}

Sticker::~Sticker() {
}


//____________________
// Private functions

bool Sticker::assignToItem() {
	Collider* assign_to = collider.getMostOverlapping(Layer::ItemLayer);
	if (assign_to != nullptr) {
		Item* target_item = assign_to->getObject().getComponent<Item>();
		if (target_item != nullptr) {
			target_item->setPrice(this->price);
			game.deleteObject(&this->object);
			return true;
		}
	}
	return false;
}

void Sticker::grab(Vector2& mouse_position) {
	Drag::grab(mouse_position);
}

void Sticker::drag(Vector2& mouse_position, float delta_time) {
	Drag::drag(mouse_position, delta_time);
}

void Sticker::drop(Vector2& mouse_position) {
	Drag::drop(mouse_position);

	// Assign price to item if overlapping and return
	if (assignToItem()) return;

	Collider* fit_to = collider.getMostOverlapping(Layer::ItemDrop);
	last_dropped = fit_to == nullptr ? last_dropped : fit_to;
	collider.fitInto(last_dropped);
	object.setParent(&last_dropped->getObject()); // Set region as parent
}
