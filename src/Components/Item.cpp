#include <memory>
#include <vector>

#include "Components/Collider.h"
#include "Components/Item.h"
#include "Core/Component.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Data/DragData.h"
#include "Data/ItemData.h"


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

void Item::updateRegionLock() {
	if (!is_owned_by_player) {
		// Only allowed in receive region
		drag_data.current_region = receive_region;
		drag_data.is_region_locked = true;
		return;
	}

	CustomerRequest request = game.getCustomerRequest();
	bool keep_in_storage = request == CustomerRequest::None;
	keep_in_storage = !keep_in_storage && request == CustomerRequest::Selling;
	bool is_in_storage = drag_data.current_region == storage_region;

	if (keep_in_storage && is_in_storage) {
		drag_data.is_region_locked = true;
		return;
	}
	drag_data.is_region_locked = false;
}

void Item::updateDroppableRegions() {
	CustomerRequest request = game.getCustomerRequest();
	bool only_inventory = request == CustomerRequest::None;
	only_inventory = !only_inventory || request == CustomerRequest::Selling;
	if (only_inventory) {
		drag_data.droppable_regions = { storage_region };
	}
	drag_data.droppable_regions = { storage_region, send_region };
}
