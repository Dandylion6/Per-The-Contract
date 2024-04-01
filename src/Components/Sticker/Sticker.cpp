#include <memory>
#include <string>
#include <vector>

#include "Components/Collider.h"
#include "Components/Customer.h"
#include "Components/Item.h"
#include "Components/Sticker/Sticker.h"
#include "Core/Component.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Data/Role.h"
#include "Managers/CustomerManager.h"
#include "Managers/DialogueManager.h"


//_______________
// Constructors

Sticker::Sticker(
	Game& game, Object& object, 
	Collider& collider, uint16_t current_price
) : 
	Drag(game, object, collider), 
	current_price(current_price),
	dialogue_manager(game.getDialogueManager()) 
{
	current_region = storage_region;
	droppable_regions = { storage_region };
}

Sticker::~Sticker() {
}


//____________________
// Private functions

bool Sticker::assignToItem() {
	Collider* assign_to = collider.getOverlapping();
	if (assign_to == nullptr) return false;

	Item* target_item = assign_to->getObject().getComponent<Item>();
	if (target_item == nullptr) return false;

	// Prevent consecutive price changing when negotiating
	std::weak_ptr<Role> latest_offer_by = target_item->getLatestOfferBy();
	if (latest_offer_by.lock() != nullptr) {
		if (*latest_offer_by.lock() == Role::Merchant) return false;
	}
	
	// If not changing price of item in storage
	Item* item_negotiating = game.getItemNegotiating();
	if (&target_item->getObject() != &storage_region->getObject()) {
		// Prevent player changing price while negotiating other item
		if (item_negotiating != nullptr && target_item != item_negotiating) {
			return false;
		}
		game.setItemNegotiating(target_item);
	}

	// Price can be assigned to item
	handleDialogue(target_item);
	target_item->setCurrentPrice(this->current_price);
	
	target_item->setLatestOfferBy(Role::Merchant);
	game.getCustomerManager().getCustomer()->reactToNegotiation(target_item);
	game.deleteObject(&this->object);
	return true;
}

void Sticker::updateDroppableRegions() {
}

void Sticker::updateRegionLock() {
}

void Sticker::drop(Vector2& mouse_position) {
	if (assignToItem()) return; // If overlapping an item
	Drag::drop(mouse_position);
}

void Sticker::handleDialogue(Item* item) {
	// Don't create dialogue if no customer
	if (game.getCustomerRequest() == CustomerRequest::None) return;
	std::weak_ptr<Role> latest_offer_by = item->getLatestOfferBy();
	bool not_negotiated = latest_offer_by.lock() == nullptr;

	bool is_first_sell_price = not_negotiated && item->getOwnedByPlayer();
	bool is_first_buy_price = not_negotiated && !item->getOwnedByPlayer();
	
	if (is_first_sell_price) {
		dialogue_manager.generateDialogue(
			Role::Merchant, "initiate_sell_price", std::to_string(current_price)
		);
		return;
	}

	if (is_first_buy_price) {
		dialogue_manager.generateDialogue(
			Role::Merchant, "initiate_buy_offer", std::to_string(current_price)
		);
		return;
	}

	dialogue_manager.generateDialogue(
		Role::Merchant, "negotiate_offer", std::to_string(current_price)
	);
}
