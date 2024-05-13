#include <cstdint>
#include <fstream>
#include <iosfwd>
#include <iterator>
#include <memory>
#include <string>

#include "SFML/Graphics/Color.hpp"

#include "Components/Collider.h"
#include "Components/Drag.h"
#include "Components/Objects/Contract.h"
#include "Components/Renderers/TextRenderer.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Core/Utility/Bounds.h"
#include "Core/Utility/Vector2.h"
#include "Data/DealData.h"
#include "Data/FontStyle.h"
#include "Data/OrderType.h"
#include "Managers/ContractManager.h"



//_______________
// Constructors

Contract::Contract(
	Game& game, 
	Object& object, 
	Collider& collider, 
	uint8_t hours,
	OrderType order_type
) : 
	Drag(game, object, collider),
	hours(hours), order_type(order_type)
{
	drag_data.current_region = receive_region;
	Object* detail_object = new Object(game, "contract_detail_display");
	detail_object->setZIndex(5);

	details_renderer = new TextRenderer(game, *detail_object, FontStyle::CourierPrime, getText());
	details_renderer->setSize(20u);
	details_renderer->addBackground(sf::Color(233u, 231u, 215u, 215u), Vector2::scale(80.f));
	details_renderer->setColor(sf::Color::Black);

	Vector2 extent = (details_renderer->getSize() + Vector2::scale(80.f)) * 0.5f;
	Vector2 window_size = game.getWindow().getSize();
	details_bounds = Bounds(extent, window_size - extent);
}

Contract::~Contract() {
}

uint8_t Contract::getHours() const {
	return this->hours;
}

OrderType Contract::getOrderType() const {
	return this->order_type;
}


//___________________
// Public functions

void Contract::update(float delta_time) {
	Drag::update(delta_time);
	details_renderer->setEnabled(is_hovering);

	if (!is_hovering) return;
	Vector2 position = Vector2::clamp(object.getPosition(), details_bounds.min, details_bounds.max);
	details_renderer->getObject().setPosition(position);
}


//____________________
// Private functions

void Contract::drop(Vector2& mouse_position) {
	Drag::drop(mouse_position);
	if (game.getDealData() == nullptr) game.startNextDeal();
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

std::string Contract::getText() {
	std::ifstream contract_stream("assets/data/contract_text.txt");
	std::string contents((std::istreambuf_iterator<char>(contract_stream)), (std::istreambuf_iterator<char>()));
	
	size_t hours_position = contents.find("[hours]");
	size_t order_position = contents.find("[order]");

	if (hours_position != std::string::npos) contents.replace(hours_position, 7u, std::to_string(hours));
	if (order_position != std::string::npos) {
		std::string line = ContractManager::getInstance()->getContractLine(order_type);
		// Temporary
		size_t insert_position = line.find("[insert]");
		if (insert_position != std::string::npos) {
			line.replace(insert_position, 8u, "400");
		}
		contents.replace(order_position, 7u, line);
	}
	return contents;
}
