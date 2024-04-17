#include <memory>
#include <vector>

#include "Components/Collider.h"
#include "Components/Objects/Item.h"
#include "Core/Component.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Data/DragData.h"
#include "Data/ItemData.h"
#include "Data/DealData.h"


//_______________
// Constructors

Item::Item(
	Game& game,
	Object& object,
	Collider& collider,
	ItemData& data
) : 
	Drag(game, object, collider),
	data(data) 
{
}

Item::~Item() {
}


//__________
// Getters

ItemData& Item::getData() const {
	return this->data;
}

bool Item::getOwnedByPlayer() const {
	return this->is_owned_by_player;
}

std::weak_ptr<Role> Item::getLatestOfferBy() const {
	return this->latest_offer_by;
}

uint16_t Item::getCurrentPrice() const {
	return this->current_price;
}

uint16_t Item::getLastPrice() const {
	return this->last_price;
}


//__________
// Setters

void Item::setOwned(bool is_owned_by_player) {
	this->is_owned_by_player = is_owned_by_player;
	latest_offer_by.reset();
}

void Item::setLatestOfferBy(Role offer_by) {
	this->latest_offer_by = std::make_shared<Role>(offer_by);
}

void Item::setCurrentPrice(uint16_t current_price) {
	this->last_price = this->current_price;
	this->current_price = current_price;
}


//____________________
// Private functions

void Item::drop(Vector2& mouse_position) {
	if (drag_data.current_region == nullptr) drag_data.current_region = storage_region;
	Drag::drop(mouse_position);

	std::shared_ptr<DealData> deal_data = game.getDealData();
	if (deal_data == nullptr) {
		game.startNextDeal();
		return;
	}

	if (deal_data->request == CustomerRequest::Buying) {
		if (deal_data->offered_item != nullptr) return;
		if (deal_data->request_id != data.item_id) return;
		deal_data->offered_item = this; // Make this offer item
		return;
	}
}

void Item::updateRegionLock() {
	// If not owned then lock in receive region
	if (!is_owned_by_player) {
		drag_data.is_region_locked = true;
		return;
	}

	std::shared_ptr<DealData> deal_data = game.getDealData();
	bool no_active_deal = deal_data == nullptr;
	bool is_in_storage = drag_data.current_region == storage_region;

	// If there is a buying request
	if (!no_active_deal && deal_data->request == CustomerRequest::Buying) {
		// If requested item is not offered yet
		if (deal_data->offered_item == nullptr && data.item_id == deal_data->request_id) {
			drag_data.is_region_locked = !is_in_storage; // Can drag if in storage
		} else {
			// Lock all items
			drag_data.is_region_locked = true;
		}
		return;
	}
	drag_data.is_region_locked = is_in_storage;
}

void Item::updateDroppableRegions() {
	std::shared_ptr<DealData> deal_data = game.getDealData();
	if (deal_data == nullptr) {
		drag_data.droppable_regions = { storage_region, drag_data.current_region };
		return;
	}

	if (deal_data->request == CustomerRequest::Selling) {
		drag_data.droppable_regions = { storage_region, receive_region };
	} else if (deal_data->request == CustomerRequest::Buying) {
		drag_data.droppable_regions = { storage_region, send_region };
	}
}
