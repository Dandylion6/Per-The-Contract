#include "Components/Cash.h"
#include "Components/Drag.h"
#include "Core/Managers/Game.h"


//_______________
// Constructors

Cash::Cash(
	Game& game,
	Object& object,
	Collider& collider,
	uint8_t value
) : Drag(game, object, collider) {
}

Cash::~Cash() {
}


//____________________
// Private functions

void Cash::updateRegionLock() {
	CustomerRequest request = game.getCustomerRequest();
	bool keep_in_storage = request == CustomerRequest::None;
	bool is_in_storage = current_region == storage_region;

	if (keep_in_storage && is_in_storage) {
		is_region_locked = true;
		return;
	}

	is_region_locked = false;
}

void Cash::updateDroppableRegions() {
	CustomerRequest request = game.getCustomerRequest();
	bool only_inventory = request == CustomerRequest::None;
	only_inventory = !only_inventory && request == CustomerRequest::Buying;
	if (only_inventory) {
		droppable_regions = { storage_region };
	}
	droppable_regions = { storage_region, send_region };
}
