#include <cstdint>
#include <memory>
#include <string>

#include "Components/Collider.h"
#include "Components/Drag.h"
#include "Components/Objects/Item.h"
#include "Components/Sticker/Sticker.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Core/Utility/Vector2.h"
#include "Data/DealData.h"
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
	current_price(current_price)
{
	drag_data.droppable_regions = { storage_region };
	drag_data.current_region = storage_region;
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
	if (!latest_offer_by.expired()) {
		if (*latest_offer_by.lock() == Role::Merchant) return false;
	}
	
	Item* deal_item = game.getDealData()->offered_item;
	bool is_deal_item = target_item == deal_item;

	// Can't change price when deal is agreed on
	if (is_deal_item && game.getDealData()->deal_agreed) return false;
	uint16_t last_price = target_item->getLastPrice();
	target_item->setCurrentPrice(this->current_price);

	// Handle negotiation
	if (is_deal_item) {
		handleDialogue(target_item, last_price);
		target_item->setLatestOfferBy(Role::Merchant);
		CustomerManager::getInstance().getCustomer()->actOnPlayerOffer();
	}

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

void Sticker::handleDialogue(Item* item, uint16_t last_price) {
	// Don't create dialogue if no active deal
	std::shared_ptr<DealData> deal_data = game.getDealData();
	if (deal_data == nullptr) return;
	bool not_negotiated = deal_data->customer_accepted_price == 0u;

	if (!not_negotiated) {
		if (item->getCurrentPrice() == last_price) {
			// Player proposing same offer
			DialogueManager::getInstance().generateDialogue(
				Role::Merchant, "restate_offer", std::to_string(current_price)
			);
		} else {
			DialogueManager::getInstance().generateDialogue(
				Role::Merchant, "negotiate_offer", std::to_string(current_price)
			);
		}
		return;
	}

	if (deal_data->request == CustomerRequest::Buying) {
		DialogueManager::getInstance().generateDialogue(
			Role::Merchant, "initiate_sell_price", std::to_string(current_price)
		);
	} else if (deal_data->request == CustomerRequest::Selling) {
		DialogueManager::getInstance().generateDialogue(
			Role::Merchant, "initiate_buy_price", std::to_string(current_price)
		);
	}
}
