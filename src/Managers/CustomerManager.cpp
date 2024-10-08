#include <algorithm>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "Components/Customer/Customer.h"
#include "Components/Objects/Cash.h"
#include "Components/Objects/Contract.h"
#include "Components/Objects/Item.h"
#include "Components/Renderers/SpriteRenderer.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Core/Utility/RandomGenerator.h"
#include "Core/Utility/Vector2.h"
#include "Data/CustomerTrait.h"
#include "Data/DealData.h"
#include "Factories/ItemFactory.h"
#include "Managers/ContractManager.h"
#include "Managers/CustomerManager.h"


CustomerManager* CustomerManager::instance = nullptr;

//_______________
// Constructors

CustomerManager::CustomerManager(Game& game) : game(game) {
	if (instance != nullptr) {
		delete this;
		return;
	}
	instance = this;
	createCustomer();

	send_region = game.getObject("send_region");
	receive_region = game.getObject("receive_region");
	storage = game.getObject("storage");
}

CustomerManager::~CustomerManager() {
}


//__________
// Getters

CustomerManager& CustomerManager::getInstance() {
	return *instance;
}

Customer* CustomerManager::getCustomer() const {
	return this->customer;
}


//___________________
// Public functions

void CustomerManager::closeDeal() {
	std::shared_ptr<DealData> deal_data = game.getDealData();
	if (deal_data->deal_agreed) {
		if (deal_data->request == CustomerRequest::Selling) {
			receiveCashForItem();
		} else if (deal_data->request == CustomerRequest::Buying) {
			customer->dropCash(deal_data->offered_item->getCurrentPrice());
			game.deleteObject(&deal_data->offered_item->getObject());
		}
	} else {
		if (deal_data->request == CustomerRequest::Selling) {
			game.deleteObject(&deal_data->offered_item->getObject());
		}
	}
	customer->leave();
	game.closeShop();
}

void CustomerManager::changeCustomer() {
	// Generate customer
	CustomerRequest new_request = generateRequest();
	CustomerTrait customer_trait = static_cast<CustomerTrait>(utils::Random::randomIndex(2));
	std::shared_ptr<DealData> new_deal = std::make_shared<DealData>(customer_trait, new_request);

	if (new_request == CustomerRequest::Selling) {
		Item* new_item = ItemFactory::getInstance().generateRandomItem();
		new_item->getObject().setZIndex(-5);
		new_deal->offered_item = new_item;
		new_deal->request_id = new_item->getData().item_id;
	} else if (new_request == CustomerRequest::Buying) {
		new_deal->request_id = findBuyRequestItem();
	}
	game.setDealData(new_deal);

	// Change sprites
	bool is_contractor = new_request == CustomerRequest::Contract;
	head_renderer->setSprite(is_contractor ? contractor_head_path : customer_head_path);
	torso_renderer->setSprite(is_contractor ? contractor_torso_path : customer_torso_path);
	customer->enter();
}


//____________________
// Private functions

CustomerRequest CustomerManager::generateRequest() {
	if (ContractManager::getInstance()->isContractorEntering()) return CustomerRequest::Contract;

	int random_number = utils::Random::generateInt(0, 1);
	CustomerRequest request = static_cast<CustomerRequest>(random_number);
	std::vector<Item*> storage_items = getStorageItems();
	
	// Can't buy item is there is none
	bool storage_empty = storage_items.size() == 0u;
	if (storage_empty && request == CustomerRequest::Buying) return CustomerRequest::Selling;
	return request;
}

std::string CustomerManager::findBuyRequestItem() {
	std::vector<std::string> storage_items;
	for (Item* item : getStorageItems()) storage_items.push_back(item->getData().item_id);
	std::sort(storage_items.begin(), storage_items.end());

	// Remove duplicates so every item has even chance of being requested
	storage_items.erase(std::unique(storage_items.begin(), storage_items.end()), storage_items.end());
	return storage_items.at(utils::Random::randomIndex(storage_items.size()));
}

std::vector<Item*> CustomerManager::getStorageItems() {
	std::vector<Item*> storage_items;
	for (Object* child : storage->getChildren()) {
		Item* item = child->getComponent<Item>();
		if (item == nullptr) continue;
		storage_items.push_back(item);
	}
	return storage_items;
}

void CustomerManager::createCustomer() {
	// Instantiate objects
	Object* customer_object = new Object(game, "customer");
	customer_object->setZIndex(-2);
	Object* torso_object = new Object(game, "torso", customer_object);
	Object* head_object = new Object(game, "head", customer_object);
	customer_object->setPosition(Vector2(300.f, 800.f));

	torso_object->setAnchor(Vector2(0.5f, 1.f));
	head_object->setAnchor(Vector2(0.5f, 1.f));

	// Add components
	torso_renderer = new SpriteRenderer(game, *torso_object);
	head_renderer = new SpriteRenderer(game, *head_object);
	customer = new Customer(game, *customer_object);
}

bool CustomerManager::receiveCashForItem() {
	uint16_t cash_needed = game.getDealData()->offered_item->getCurrentPrice();
	uint16_t cash_deposited = 0u;

	std::vector<Cash*> cash_to_give;
	for (Object* object : send_region->getChildren()) {
		Cash* cash = object->getComponent<Cash>();
		if (cash == nullptr) continue;

		cash_deposited += cash->getValue();
		cash_to_give.push_back(cash);
		if (cash_deposited >= cash_needed) break; // Exit if payment is met
	}

	// Give customer owed cash
	for (Cash* cash : cash_to_give) game.deleteObject(&cash->getObject());
	cash_to_give.clear();

	// Transfer ownership of items
	for (Object* object : receive_region->getChildren()) {
		Item* item = object->getComponent<Item>();
		if (item == nullptr) continue;
		item->setOwned(true); // Player now owns the item
	}

	// Get change back if needed
	if (cash_deposited > cash_needed) {
		uint16_t change_value = cash_deposited - cash_needed;
		customer->dropCash(change_value);
	}
	return true;
}

