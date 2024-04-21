#include <cstdint>
#include <memory>

#include "Components/Buttons/AcceptDealButton.h"
#include "Components/Buttons/Button.h"
#include "Components/Collider.h"
#include "Components/Objects/Cash.h"
#include "Components/Objects/Contract.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Core/Utility/Vector2.h"
#include "Data/DealData.h"
#include "Data/Role.h"
#include "Managers/ContractManager.h"
#include "Managers/CustomerManager.h"
#include "Managers/DialogueManager.h"


//_______________
// Constructors

AcceptDealButton::AcceptDealButton(
	Game& game, Object& object, Collider& collider
) : 
	Button(game, object, collider) 
{
	send_region = game.getObject("send_region");
}

AcceptDealButton::~AcceptDealButton() {
}


//____________________
// Private functions

void AcceptDealButton::buttonPressed() {
	object.setScale(Vector2::scale(0.9f));
	std::shared_ptr<DealData> deal_data = game.getDealData();

	if (deal_data == nullptr) return;
	if (!deal_data->deal_started) return;

	if (deal_data->request == CustomerRequest::Contract) {
		Contract* contract = ContractManager::getInstance()->getCurrentContract();
		if (contract->getObject().getParent() != send_region) return;
		CustomerManager::getInstance().closeDeal();
		return;
	}

	if (deal_data->customer_accepted_price == nullptr) return;
	if (deal_data->request == CustomerRequest::Selling && !depositedCash()) return;

	deal_data->deal_agreed = true;
	DialogueManager::getInstance().generateDialogue(Role::Merchant, "accept_deal");
	CustomerManager::getInstance().closeDeal();
}

void AcceptDealButton::buttonReleased() {
	object.setScale(Vector2::scale(1.f));
}

bool AcceptDealButton::depositedCash() {
	uint16_t cash_needed = game.getDealData()->offered_item->getCurrentPrice();
	uint16_t cash_deposited = 0u;

	for (Object* object : send_region->getChildren()) {
		Cash* cash = object->getComponent<Cash>();
		if (cash == nullptr) continue;

		cash_deposited += cash->getValue();
		if (cash_deposited >= cash_needed) return true;
	}
	return false;
}

