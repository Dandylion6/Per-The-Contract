#include <memory>

#include "Components/Buttons/Button.h"
#include "Components/Buttons/DeclineDealButton.h"
#include "Components/Collider.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Core/Utility/Vector2.h"
#include "Data/DealData.h"
#include "Data/Role.h"
#include "Managers/CustomerManager.h"
#include "Managers/DialogueManager.h"


//______________
// Custructors

DeclineDealButton::DeclineDealButton(
	Game& game, Object& object, Collider& collider
) : 
	Button(game, object, collider) 
{
}

DeclineDealButton::~DeclineDealButton() {
}


//___________________
// Private function

void DeclineDealButton::buttonPressed() {
	object.setScale(Vector2::scale(0.9f));
	std::shared_ptr<DealData> deal_data = game.getDealData();

	if (deal_data == nullptr) return;
	if (!deal_data->deal_started) return;

	DialogueManager::getInstance().generateDialogue(Role::Merchant, "decline_deal");
	CustomerManager::getInstance().closeDeal();
}

void DeclineDealButton::buttonReleased() {
	object.setScale(Vector2::scale(1.f));
}
