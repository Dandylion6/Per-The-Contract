#include <cstdint>
#include <cstdlib>
#include <memory>
#include <string>

#include "SFML/Graphics/Color.hpp"

#include "Components/Collider.h"
#include "Components/Drag.h"
#include "Components/Objects/Item.h"
#include "Components/Renderers/TextRenderer.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Core/Utility/Math.h"
#include "Core/Utility/Vector2.h"
#include "Data/DealData.h"
#include "Data/FontStyle.h"
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
	Object* price_object = new Object(game, "price_display");
	Object* profit_object = new Object(game, "profit_display");
	price_object->setAnchor(Vector2(0.5f, 1.f));
	profit_object->setAnchor(Vector2(0.5f, 1.f));
	price_object->setZIndex(4);
	profit_object->setZIndex(4);

	price_display = new TextRenderer(game, *price_object, FontStyle::Roboto, "<- Make first offer");
	profit_display = new TextRenderer(game, *profit_object, FontStyle::Roboto);
}

Item::~Item() {
	game.deleteObject(&price_display->getObject());
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
	if (is_owned_by_player) buy_price = current_price;
	latest_offer_by.reset();
}

void Item::setLatestOfferBy(Role offer_by) {
	this->latest_offer_by = std::make_shared<Role>(offer_by);
}

void Item::setCurrentPrice(uint16_t current_price) {
	this->last_price = this->current_price;
	price_changing = last_price != current_price;
	price_change_time = 0.f;
	this->current_price = current_price;
}

void Item::update(float delta_time) {
	Drag::update(delta_time);
	bool in_storage = object.getParent() == &storage_region->getObject();
	bool display = is_hovering || !in_storage;

	std::shared_ptr<DealData> deal_data = game.getDealData();
	if (deal_data != nullptr && deal_data->request == CustomerRequest::Buying) {
		// If requested item is not offered yet
		if (deal_data->offered_item == nullptr && data.item_id == deal_data->request_id) {
			price_display->getObject().setEnabled(true);
			price_display->setText("Drop item on countertop");
			return;
		}
	}

	if (display) {
		updatePriceDisplay(delta_time);
		updateProfitDisplay(delta_time);
	} else profit_display->getObject().setEnabled(false);
}


//____________________
// Private functions

void Item::drop(Vector2& mouse_position) {
	if (drag_data.current_region == nullptr) drag_data.current_region = storage_region;
	Drag::drop(mouse_position);

	std::shared_ptr<DealData> deal_data = game.getDealData();
	if (deal_data == nullptr) {
		this->latest_offer_by.reset();
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

void Item::updatePriceDisplay(float delta_time) {
	Vector2 offset = Vector2(0.f, (collider.getSize().y * -0.5f) - 20.f);
	price_display->getObject().setPosition(object.getPosition() + offset);

	if (!price_changing) return;
	price_change_time = utils::clamp(price_change_time + delta_time, 0.f, price_change_duration);
	float time = price_change_time / price_change_duration;
	uint16_t display_price = static_cast<uint16_t>(utils::outQuad(last_price, current_price, time));
	price_display->setText(std::to_string(display_price) + "$");
}

void Item::updateProfitDisplay(float delta_time) {
	bool should_display = shouldDisplayProfit();
	profit_display->getObject().setEnabled(should_display);
	if (!should_display) return;

	Vector2 offset = Vector2(0.f, (collider.getSize().y * -0.5f) - 50.f);
	profit_display->getObject().setPosition(object.getPosition() + offset);

	if (!price_changing) return;
	float time = price_change_time / price_change_duration;
	int last_profit = last_price - buy_price;
	int new_profit = current_price - buy_price;

	int profit = static_cast<int>(std::abs(utils::outQuad(last_profit, new_profit, time)));
	std::string sign = profit < 0 ? "-" : "+";
	profit_display->setText(sign + std::to_string(profit) + "$");
	profit_display->setColor(profit > 0 ? profit_colour : loss_colour);

	if (price_change_time < price_change_duration) return;
	price_changing = false;
	price_change_time = 0.f;
}

bool Item::shouldDisplayProfit() const {
	if (!is_owned_by_player) return false;
	std::shared_ptr<DealData> deal_data = game.getDealData();
	if (deal_data == nullptr) return false;
	if (deal_data->request != CustomerRequest::Buying) return false;
	if (deal_data->offered_item != this) return false;
	return true;
}
