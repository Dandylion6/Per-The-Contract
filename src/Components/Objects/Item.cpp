#include <cstdint>
#include <memory>
#include <string>

#include "Components/Collider.h"
#include "Components/Drag.h"
#include "Components/Objects/Item.h"
#include "Components/Renderers/TextRenderer.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Core/Utility/Vector2.h"
#include "Data/DealData.h"
#include "Data/ItemData.h"
#include "Data/Role.h"
#include "Managers/DialogueManager.h"


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
	Object* display_object = new Object(game, "price_display");
	display_object->setAnchor(Vector2(0.5f, 1.f));
	price_display = new TextRenderer(game, *display_object, FontStyle::Roboto);
	display_object->setEnabled(false);
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
	price_display->setText("Price: " + std::to_string(current_price));
}

void Item::update(float delta_time) {
	Drag::update(delta_time);
	bool display = current_price != 0u && is_hovering;
	Vector2 offset = Vector2(0.f, (collider.getSize().y * -0.5f) - 20.f);
	price_display->getObject().setPosition(object.getPosition() + offset);
	price_display->getObject().setEnabled(display);
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

	if (drag_data.current_region != send_region) return;
	if (deal_data->request == CustomerRequest::Buying) {
		if (deal_data->offered_item != nullptr) return;
		if (deal_data->request_id != data.item_id) return;
		deal_data->offered_item = this; // Make this offer item
		DialogueManager::getInstance().generateDialogue(Role::Merchant, "show_requested_item");
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
			drag_data.is_region_locked = is_in_storage;
		}
	} else {
		drag_data.is_region_locked = is_in_storage;
	}
}

void Item::updateDroppableRegions() {
	std::shared_ptr<DealData> deal_data = game.getDealData();
	if (deal_data != nullptr && deal_data->request == CustomerRequest::Buying) {
		drag_data.droppable_regions = { storage_region, send_region };
	} else {
		drag_data.droppable_regions = {
		storage_region, send_region, receive_region
		};
	}
}
