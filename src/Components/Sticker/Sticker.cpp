#include <string>

#include "Components/Collider.h"
#include "Components/Item.h"
#include "Components/Sticker/Sticker.h"
#include "Core/Component.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Data/Role.h"
#include "Managers/DialogueManager.h"


//_______________
// Constructors

Sticker::Sticker(
	Game& game, Object& object, 
	Collider& collider, uint16_t price
) : 
	Drag(game, object, collider), 
	price(price),
	dialogue_manager(game.getDialogueManager()) 
{
	last_dropped = collider.getMostOverlapping(Layer::ItemDrop);
}

Sticker::~Sticker() {
}


//____________________
// Private functions

bool Sticker::assignToItem() {
	Collider* assign_to = collider.getMostOverlapping(Layer::Dragable);
	if (assign_to == nullptr) return false;

	// Set price to object
	Item* target_item = assign_to->getObject().getComponent<Item>();
	if (target_item == nullptr) return false;
	target_item->setPrice(this->price);
	
	handleDialogue(target_item);

	game.deleteObject(&this->object);
	return true;
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

void Sticker::handleDialogue(Item* item) {
	bool price_not_set = item->getPrice() == 0u;
	bool is_first_sell_price = price_not_set && item->getOwnedByPlayer();
	
	if (is_first_sell_price) {
		dialogue_manager.generateDialogue(
			Role::Merchant, "initiate_sell_price", std::to_string(price)
		);
		return;
	} 

	dialogue_manager.generateDialogue(
		Role::Merchant, "negotiate_offer", std::to_string(price)
	);
}
