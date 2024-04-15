#include <memory>

#include "Components/Buttons/AcceptDealButton.h"
#include "Components/Buttons/Button.h"
#include "Data/DealData.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Core/Utility/Vector2.h"
#include "Managers/DialogueManager.h"
#include "Data/Role.h"


//_______________
// Constructors

AcceptDealButton::AcceptDealButton(
	Game& game, Object& object, Collider& collider
) : 
	Button(game, object, collider) 
{
}

AcceptDealButton::~AcceptDealButton() {
}


//____________________
// Private functions

void AcceptDealButton::buttonPressed() {
	object.setScale(Vector2::scale(0.9f));
	std::shared_ptr<DealData> deal_data = game.getDealData();

	if (deal_data == nullptr) return;
	if (deal_data->customer_accepted_price == nullptr) return;
	if (!deal_data->deal_started) return;

	DialogueManager::getInstance().generateDialogue(Role::Merchant, "accept_deal");
	game.endCurrentDeal(true);
}

void AcceptDealButton::buttonReleased() {
	object.setScale(Vector2::scale(1.f));
}

