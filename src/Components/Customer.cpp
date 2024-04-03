#include <cmath>
#include <memory>
#include <string>

#include "Components/Customer.h"
#include "Components/CustomerAnimator.h"
#include "Components/Item.h"
#include "Core/Component.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Core/Utility/RandomGenerator.h"
#include "Core/Utility/Vector2.h"
#include "Data/Role.h"
#include "Factories/ItemFactory.h"
#include "Managers/DialogueManager.h"


//_______________
// Constructors

Customer::Customer(
	Game& game, Object& object
) : 
	Component(game, object), 
	dialogue_manager(game.getDialogueManager()) 
{
	animator = new CustomerAnimator(game, object);
	receive_region = game.getObject("receive_region");
}

Customer::~Customer() {
}


//__________
// Setters

void Customer::setCharacter(std::weak_ptr<CharacterData> character) {
	this->character = character;
}

void Customer::setCustomer(
	CustomerTrait trait, uint16_t funds, float willingness_factor
) {
	this->trait = trait;
	this->funds = funds;
	this->negotiability_factor = negotiability_trait.find(trait)->second;
	this->acceptable_range_factor = acceptable_range_trait.find(trait)->second;
	this->willingness_factor = willingness_factor;
}


//___________________
// Public functions

void Customer::reactToPriceOffered(Item* item) {
	CustomerRequest request = game.getCustomerRequest();
	uint16_t offered_price = item->getCurrentPrice();

	// Check if in acceptable range
	bool in_price_range = isAcceptablePrice(offered_price);
	if (in_price_range) {
		if (willAcceptDeal()) {
			// Accept deal
			return;
		}
		// Negotiate instead
		placeNewPriceOffer(item);
		negotiability_factor -= 0.15f;
		return;
	}

	// Did customer already give a price offer?
	bool did_give_offer = item->getLastPrice() != 0u;
	if (!did_give_offer) {
		// Let customer place an offer
		placeNewPriceOffer(item);
		return;
	}

	if (willNegotiate()) {
		// Place new offer
		placeNewPriceOffer(item);

		float acceptable = static_cast<float>(acceptable_price);
		float proportional_difference = offered_price / acceptable;
		float offer_penalty = proportional_difference - 1.f;

		if (request == CustomerRequest::Selling) {
			offer_penalty = 1.f - proportional_difference;
		}

		// If new offer is too absurd willingness will reduce
		willingness_factor -= offer_penalty * 0.35f;
		negotiability_factor -= 0.1f; // Linear reduction
		return;
	}

	// Otherwise restate last acceptable price 
	// Decline outright if willingness is too low or can't continue
}

void Customer::enter() {
	animator->setAnimation(CustomerAnimState::Entering);
	dialogue_manager.generateDialogue(Role::Merchant, "greeting");
}

void Customer::leave() {
	animator->setAnimation(CustomerAnimState::Leaving);
	game.setCustomerRequest(CustomerRequest::None);
}

void Customer::update(float delta_time) {
	// Only continue when ready for interactions
	if (animator->getAnimationState() != CustomerAnimState::Idling) return;
	
	// Generate a customer request if not already
	bool no_request = game.getCustomerRequest() == CustomerRequest::None;
	if (no_request) generateRequest();
}


//____________________
// Private functions

void Customer::generateRequest() {
	// TODO: Request based on inventory, needs and funds
	int random_number = utils::Random::generateInt(1, 3);

	CustomerRequest request = CustomerRequest::Selling;
	game.setCustomerRequest(request); // Do selling for now

	switch (request) {
		case CustomerRequest::Buying:
		{
			// TODO
			break;
		}
		case CustomerRequest::Selling:
		{
			Item* to_sell = generateItem();
			placeSellOffer(to_sell);
			break;
		}
		case CustomerRequest::Trading:
		{
			// TODO
			break;
		}
	}
}

Item* Customer::generateItem() {
	Item* item = game.getItemFactory().generateRandomItem();
	Object& object = item->getObject();
	object.setParent(receive_region);

	// TODO: Generate a perceived item value
	// if knowledeable will reflect more closely to market value
	perceived_item_value = item->getData().market_value;
	return item;
}

void Customer::placeSellOffer(Item* to_sell) {
	// TODO: Animate item sliding in

	int random_x = utils::Random::generateInt(
		-drop_range, drop_range
	);
	int random_y = utils::Random::generateInt(
		-drop_range, drop_range
	);
	to_sell->getObject().setLocalPosition(Vector2(random_x, random_y));
	dialogue_manager.generateDialogue(Role::Customer, "selling");
}

void Customer::placeNewPriceOffer(Item* item) {
	CustomerRequest request = game.getCustomerRequest();
	uint16_t offer = item->getCurrentPrice();
	float price_change_factor = 0.f;

	uint16_t range = perceived_item_value * acceptable_range_factor;
	int difference = (perceived_item_value + range) - offer;
	if (request == CustomerRequest::Selling) {
		difference = (perceived_item_value - range) - offer;
	}
	// Get a base change factor to get to limit
	uint16_t to_reach_limit = std::max(difference, 0);
	price_change_factor = to_reach_limit / offer;

	// Negotiators are willing to push the price more
	price_change_factor += negotiability_factor * 0.65f;
	// More complecent if high willingness
	price_change_factor -= (willingness_factor * 0.25f) - 0.15f;
	price_change_factor += utils::Random::generateFloat(-0.1f, 0.1f);

	uint16_t price_change = offer * price_change_factor;
	uint16_t new_offer = offer - price_change; // Can't exceed funds when buying
	new_offer = std::min(new_offer, funds);
	if (request == CustomerRequest::Selling) {
		new_offer = offer + price_change;
	}
	new_offer = ((new_offer + 5u) / 10u) * 10u; // Round last number
	item->setCurrentPrice(new_offer);
	dialogue_manager.generateDialogue(Role::Customer, "negotiate_offer");
}

bool Customer::isAcceptablePrice(uint16_t offered_price) const {
	CustomerRequest request = game.getCustomerRequest();
	uint16_t range = perceived_item_value * acceptable_range_factor;
	if (request == CustomerRequest::Selling) {
		uint16_t minimum = perceived_item_value - range;
		return offered_price >= minimum;
	} else {
		uint16_t maximum = perceived_item_value + range;
		return offered_price <= maximum;
	}
}

bool Customer::willAcceptDeal() const {
	// Certain customers might be more willing to accept or negotiate futher
	float accept_factor = 1.f - (negotiability_factor * 0.7f);
	float accept_value = utils::Random::generateFloat(0.f, accept_factor);

	float acceptance_threshold = negotiability_factor * 0.3f;
	if (negotiability_factor < 0.4f) {
		acceptance_threshold -= 0.3f; // Low negotiability will more likely accept
	}

	// If willingness is low they are more likely to accept instead of continuing
	acceptance_threshold -= (willingness_factor * 0.4f) - 0.3f;
	return accept_value > acceptance_threshold;
}

bool Customer::willNegotiate() const {
	// The chance at negotiating influenced by willingness
	float negotiate_factor = willingness_factor * 0.8f;
	float negotiate_value = utils::Random::generateFloat(0.f, negotiate_factor);

	// A high negiability is almost garanteed to negotiate
	float threshold = 1.f - negotiability_factor;
	return negotiate_value > threshold;
}