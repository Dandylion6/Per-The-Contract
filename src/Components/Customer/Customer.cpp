#include <cstdint>
#include <memory>
#include <string>

#include "Components/Customer/Customer.h"
#include "Components/Customer/CustomerAnimator.h"
#include "Components/Objects/Cash.h"
#include "Components/Objects/Contract.h"
#include "Components/Objects/Item.h"
#include "Core/Component.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Core/Utility/RandomGenerator.h"
#include "Core/Utility/Vector2.h"
#include "Data/DealData.h"
#include "Data/ItemData.h"
#include "Data/Role.h"
#include "Factories/CashFactory.h"
#include "Factories/ItemFactory.h"
#include "Managers/ContractManager.h"
#include "Managers/DialogueManager.h"


//_______________
// Constructors

Customer::Customer(Game& game, Object& object) : Component(game, object) {
	animator = new CustomerAnimator(game, object);
	storage = game.getObject("storage");
	receive_region = game.getObject("receive_region");
}

Customer::~Customer() {
}


//___________________
// Public functions

void Customer::actOnPlayerOffer() {
	thinking_time = utils::Random::generateFloat(1.f, 1.5f);
	is_thinking = true;
}

void Customer::dropCash(uint16_t value) {
	for (Cash* cash : CashFactory::getInstance().createCash(value)) {
		Object& cash_object = cash->getObject();
		cash_object.setParent(receive_region);

		Vector2 drop_position = receive_region->getPosition();
		cash->getObject().setPosition(drop_position - Vector2(0.f, 150.f));
		drop_position += utils::Random::randomRadius(drop_radius);
		cash->move_to(drop_position);
	}
}

void Customer::enter() {
	animator->setAnimation(CustomerAnimState::Entering);
	brain = brain_map.find(game.getDealData()->customer_trait)->second;
	brain->onEnter();
}

void Customer::leave() {
	animator->setAnimation(CustomerAnimState::None);
	if (game.getDealData()->request == CustomerRequest::Contract) {
		ContractManager::getInstance()->contractorLeave();
	}
	brain->onLeave();
}

void Customer::update(float delta_time) {
	// Only continue when ready for interactions
	if (animator->getAnimationState() != CustomerAnimState::Idling) return;

	// State request
	std::shared_ptr<DealData> deal_data = game.getDealData();
	if (!deal_data->deal_started) {
		handleRequest();
		deal_data->deal_started = true;
	}

	if (is_thinking && thinking_time <= 0.f) {
		is_thinking = false;
		std::string insert;
		std::string line = brain->actOnPlayerOffer(insert);
		if (insert.empty()) {
			DialogueManager::getInstance().generateDialogue(Role::Customer, line);
		} else {
			DialogueManager::getInstance().generateDialogue(Role::Customer, line, insert);
		}
	} else if (is_thinking) {
		thinking_time -= delta_time;
	}
}


//____________________
// Private functions

#include<iostream>

void Customer::handleRequest() {
	std::shared_ptr<DealData> deal_data = game.getDealData();
	if (deal_data->request != CustomerRequest::Contract) {
		std::string insert;
		std::string line = brain->stateRequest(insert);
		if (insert.empty()) DialogueManager::getInstance().generateDialogue(Role::Customer, line);
		else DialogueManager::getInstance().generateDialogue(Role::Customer, line, insert);
		DialogueManager::getInstance().generateDialogue(Role::Customer, "entry_remark");
	}

	switch (deal_data->request) {
		case CustomerRequest::Buying: {
			ItemData& item_data = ItemFactory::getInstance().getItemData(deal_data->request_id);
			break;
		}
		case CustomerRequest::Selling: {
			placeSellItem();
			break;
		}
		case CustomerRequest::Contract: {
			if (!ContractManager::getInstance()->isRetrievingContract()) {
				Contract* contract = placeNewContract();
				DialogueManager::getInstance().generateDialogue(
					Role::Customer, "contract", std::to_string(contract->getHours())
				);
				leave();
				game.closeShop();
			} else DialogueManager::getInstance().generateDialogue(Role::Customer, "contract_take");
			break;
		}
	}
}

Contract* Customer::placeNewContract() {
	Contract* contract = ContractManager::getInstance()->generateContract();
	contract->getObject().setParent(receive_region);

	Vector2 drop_position = receive_region->getPosition();
	contract->getObject().setPosition(drop_position - Vector2(0.f, 150.f));
	drop_position += utils::Random::randomRadius(drop_radius);
	contract->move_to(drop_position);
	return contract;
}

void Customer::placeSellItem() {
	Item* item = game.getDealData()->offered_item;
	item->getObject().setParent(receive_region);

	Vector2 drop_position = receive_region->getPosition();
	item->getObject().setPosition(drop_position - Vector2(0.f, 150.f));
	drop_position += utils::Random::randomRadius(drop_radius);
	item->move_to(drop_position);
}