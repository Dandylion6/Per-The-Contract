#include "Components/Objects/Cash.h"
#include "Components/Drag.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Managers/CustomerManager.h"
#include "Data/DealData.h"


//_______________
// Constructors

Cash::Cash(
	Game& game,
	Object& object,
	Collider& collider,
	uint8_t value
) : 
	Drag(game, object, collider) 
{
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

void Cash::drop(Vector2& mouse_position) {
	if (drag_data.current_region == nullptr) drag_data.current_region = storage_region;
	Drag::drop(mouse_position);
	if (game.getDealData() != nullptr) return;
	game.startNextDeal();
}

void Cash::updateRegionLock() {
	bool not_in_deal = game.getDealData() == nullptr;
	bool can_pay = !not_in_deal && game.getDealData()->request == CustomerRequest::Selling;
	bool is_in_storage = drag_data.current_region == storage_region;
	drag_data.is_region_locked = (not_in_deal || !can_pay) && is_in_storage;
}

void Cash::updateDroppableRegions() {
	std::shared_ptr<DealData> deal_data = game.getDealData();
	if (deal_data != nullptr && deal_data->request == CustomerRequest::Selling) {
		drag_data.droppable_regions = { storage_region, send_region };
	} else {
		drag_data.droppable_regions = { storage_region, drag_data.current_region };
	}
}
