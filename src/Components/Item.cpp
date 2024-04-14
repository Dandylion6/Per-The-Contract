#include <memory>
#include <vector>

#include "Components/Collider.h"
#include "Components/Item.h"
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
	setCurrentPrice(0u);
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
	Drag::drop(mouse_position);
	// Handle buying logic
	std::shared_ptr<DealData> deal_data = game.getDealData();
	if (deal_data == nullptr) return;
	if (deal_data->request != CustomerRequest::Buying) return;

	// Presented item is new deal item
	// Might add customer rejecting
	bool accept_any_item = deal_data->item == nullptr;
	if (!accept_any_item) return;
	deal_data->item = this;
}

void Item::updateRegionLock() {
	if (!is_owned_by_player) {
		// Only allowed in receive region
		drag_data.current_region = receive_region;
		drag_data.is_region_locked = true;
		return;
	}

	std::shared_ptr<DealData> deal_data = game.getDealData();
	bool not_in_deal = deal_data == nullptr;
	bool keep_in_storage = not_in_deal || deal_data->request != CustomerRequest::Buying;
	bool is_in_storage = drag_data.current_region == storage_region;

	if (!keep_in_storage) { // This means customer is buying
		std::shared_ptr<DealData> deal_data = game.getDealData();
		// Can accept any player item
		if (deal_data->item == nullptr) {
			drag_data.is_region_locked = !is_in_storage;
			return;
		}
		// Item is deal target
		bool is_target = this == deal_data->item;
		drag_data.is_region_locked = is_target ? is_in_storage : !is_in_storage;
		return;
	}

	drag_data.is_region_locked = keep_in_storage && is_in_storage;
}

void Item::updateDroppableRegions() {
	bool not_in_deal = game.getDealData() == nullptr;
	bool is_selling = !not_in_deal && game.getDealData()->request == CustomerRequest::Buying;
	if (not_in_deal || !is_selling) {
		drag_data.droppable_regions = { storage_region };
		return;
	}
	drag_data.droppable_regions = { storage_region, send_region };
}
