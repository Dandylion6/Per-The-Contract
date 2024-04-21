#include <cstdint>
#include <memory>

#include "Components/Collider.h"
#include "Components/Drag.h"
#include "Components/Objects/Contract.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Core/Utility/Vector2.h"
#include "Data/DealData.h"
#include "Managers/ContractManager.h"


//_______________
// Constructors

Contract::Contract(
	Game& game, 
	Object& object, 
	Collider& collider, 
	uint8_t hours
) : 
	Drag(game, object, collider),
	hours(hours)
{
	drag_data.current_region = receive_region;
}

Contract::~Contract() {
}

uint8_t Contract::getHours() const {
	return this->hours;
}


//___________________
// Public functions

bool Contract::isCompleted() {
	return true;
}


//____________________
// Private functions

void Contract::drop(Vector2& mouse_position) {
	Drag::drop(mouse_position);
	if (game.getDealData() != nullptr) return;
	game.startNextDeal();
}

void Contract::updateRegionLock() {
	std::shared_ptr<DealData> deal_data = game.getDealData();
	bool is_in_storage = drag_data.current_region == storage_region;
	if (deal_data == nullptr || deal_data->request != CustomerRequest::Contract) {
		drag_data.is_region_locked = is_in_storage;
	} else {
		drag_data.is_region_locked = !is_in_storage; // Give contract
	}
}

void Contract::updateDroppableRegions() {
	std::shared_ptr<DealData> deal_data = game.getDealData();
	if (deal_data == nullptr) {
		drag_data.droppable_regions = { storage_region, drag_data.current_region };
		return;
	}

	if (deal_data->request == CustomerRequest::Contract) {
		bool is_in_storage = drag_data.current_region == storage_region;
		drag_data.droppable_regions = { storage_region, is_in_storage ? send_region : receive_region };
	} else {
		drag_data.droppable_regions = { storage_region, drag_data.current_region };
	}
}
