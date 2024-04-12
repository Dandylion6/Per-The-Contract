#include "Components/Cash.h"
#include "Components/Drag.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Managers/CustomerManager.h"


//_______________
// Constructors

Cash::Cash(
	Game& game,
	Object& object,
	Collider& collider,
	uint8_t value
) : Drag(game, object, collider) {
	this->value = value;
}

Cash::~Cash() {
}


//__________
// Getters

uint8_t Cash::getValue() const {
	return this->value;
}


//____________________
// Private functions

#include <iostream>

void Cash::drop(Vector2& mouse_position) {
	Drag::drop(mouse_position);
	if (drag_data.current_region != send_region) return;
	// TODO: Check if accepted
	CustomerManager::getInstance().closeShop(true);
}

void Cash::updateRegionLock() {
	CustomerRequest request = game.getCustomerRequest();
	bool keep_in_storage = request == CustomerRequest::None;
	bool is_in_storage = drag_data.current_region == storage_region;
	drag_data.is_region_locked = keep_in_storage && is_in_storage;

	if (request != CustomerRequest::Buying) return;

}

void Cash::updateDroppableRegions() {
	CustomerRequest request = game.getCustomerRequest();
	bool only_inventory = request == CustomerRequest::None;
	only_inventory = only_inventory || request == CustomerRequest::Buying;
	if (only_inventory) {
		drag_data.droppable_regions = { storage_region };
	}
	drag_data.droppable_regions = { storage_region, send_region };
}
