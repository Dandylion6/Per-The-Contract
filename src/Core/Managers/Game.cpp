#include <algorithm>
#include <cstdint>
#include <list>
#include <memory>
#include <string>
#include <vector>

#include "SFML/Graphics/RenderWindow.hpp"

#include "Components/Objects/Cash.h"
#include "Components/Renderers/SpriteRenderer.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Core/Utility/RandomGenerator.h"
#include "Core/Utility/Vector2.h"
#include "Data/DealData.h"
#include "Data/Role.h"
#include "Factories/CashFactory.h"
#include "Factories/DealClosureFactory.h"
#include "Factories/EnvironmentFactory.h"
#include "Factories/ItemFactory.h"
#include "Factories/StickerFactory.h"
#include "Factories/StickerPrinterFactory.h"
#include "Managers/ContractManager.h"
#include "Managers/CustomerManager.h"
#include "Managers/DialogueManager.h"
#include "Managers/ShopDoorManager.h"
#include "Components/Objects/Item.h"


//_______________
// Constructors

Game::Game(sf::RenderWindow& window) : window(window) {
	new EnvironmentFactory(*this);
	new StickerFactory(*this);
	new StickerPrinterFactory(*this);
	new DealClosureFactory(*this);

	new CashFactory(*this);
	new ItemFactory(*this);

	new DialogueManager(*this);
	new ContractManager(*this);
	new CustomerManager(*this);
	new ShopDoorManager(*this);

	send_region = getObject("send_region");
	receive_region = getObject("receive_region");

	InstantiateGame();
}

Game::~Game() {
	// Deleting because it is good practice, but would be done anyway on exit
	for (auto it = objects_to_delete.begin(); it != objects_to_delete.end();) {
		Object* object = *it;
		object->setParent(nullptr);
		objects.remove(object);
		delete object;
		++it;
	}
	objects_to_delete.clear();
	objects.clear();
}


//__________
// Getters

sf::RenderWindow& Game::getWindow() const {
	return this->window;
}

const std::list<Object*>& Game::getObjects() const {
	return this->objects;
}

Object* Game::getObject(std::string name) const {
	for (Object* object : objects) {
		if (object->getName() == name) return object;
	}
	return nullptr;
}

std::shared_ptr<DealData> Game::getDealData() const {
	return this->deal_data;
}

uint8_t Game::getTimeOfDay() const {
	return this->time_of_day;
}

float Game::getTimeOfDayFloat() const {
	float minutes = since_last_hour / (game_hours_in_minutes * 60.f);
	return static_cast<float>(this->time_of_day + minutes);
}


//__________
// Setters

void Game::setDealData(std::shared_ptr<DealData> deal_data) {
	this->deal_data.reset();
	if (deal_data == nullptr) return;
	this->deal_data.swap(deal_data);
}


//___________________
// Public functions

void Game::resortObject(Object* object) {
	auto it = std::find(objects.begin(), objects.end(), object);
	if (it != objects.end()) {
		objects.erase(it);
		addObject(object);
	}
}

void Game::update(float delta_time) {
	if (ready_for_next && !ShopDoorManager::getInstance().isMoving()) {
		ready_for_next = false;

		DialogueManager::getInstance().clearDialogue();
		CustomerManager::getInstance().changeCustomer();
		ShopDoorManager::getInstance().openDoor();

		if (deal_data->request == CustomerRequest::Contract) {
			DialogueManager::getInstance().generateDialogue(Role::Merchant, "greeting_contractor");
		} else {
			DialogueManager::getInstance().generateDialogue(Role::Merchant, "greeting");
		}
	}

	// Update objects
	for (Object* object : getObjectsSorted()) {
		if (object->getEnabled()) object->update(delta_time);
	}

	deleteObjects();

	// Update time of day
	if (deal_data == nullptr) return; // Pause if not doing a deal
	since_last_hour += delta_time;
	if (since_last_hour >= game_hours_in_minutes * 60.f) {
		time_of_day = (time_of_day + 1) % 24;
		ContractManager::getInstance()->contractorArriveCheck();
		since_last_hour = 0.f;
	}
}

void Game::startNextDeal() {
	if (deal_data != nullptr) return;

	// Make sure next deal can start
	if (send_region->getChildren().size() > 0u) return;
	if (receive_region->getChildren().size() > 0u) return;
	ready_for_next = true;
}

void Game::closeShop() {
	deal_data.reset();
	ShopDoorManager::getInstance().closeDoor();
	startNextDeal(); // Start next deal if possible
}

void Game::addObject(Object* object) {
	objects.push_back(object);
}

void Game::deleteObject(std::string name) {
	deleteObject(getObject(name));
}

void Game::deleteObject(Object* object) {
	object->setParent(nullptr);
	for (Object* child : object->getChildren()) {
		if (child != nullptr) objects_to_delete.push_back(child);
	}
	objects_to_delete.push_back(object);
}


//____________________
// Private functions

void Game::InstantiateGame() {
	startNextDeal();

	Object* storage_object = getObject("storage");
	Vector2 size = storage_object->getComponent<SpriteRenderer>()->getSize();

	// Create starting cash
	const std::vector<Cash*>& starting_cash = CashFactory::getInstance().createCash(380u);
	for (Cash* cash : starting_cash) {
		cash->getObject().setParent(storage_object);
		Vector2 offset = utils::Random::randomRadius(70.f);
		Vector2 position = Vector2(offset.x - size.x + 120.f, 120.f + offset.y);
		cash->getObject().setLocalPosition(position);
	}

	// Create starting items
	for (int i = 0; i < 2; i++) {
		Item* item = ItemFactory::getInstance().generateRandomItem();
		item->getObject().setParent(storage_object);

		Vector2 offset = utils::Random::randomRadius(100.f);
		Vector2 position = Vector2(offset.x - 250.f, 200.f + offset.y);
		item->getObject().setLocalPosition(position);
		item->setOwned(true); // Player owns the item
	}
}

std::list<Object*> Game::getObjectsSorted() const {
	std::list<Object*> sorted_objects(objects.begin(), objects.end());
	// Sorting based on Z-index
	sorted_objects.sort(compareZIndex);
	return sorted_objects;
}

void Game::deleteObjects() {
	for (auto it = objects_to_delete.begin(); it != objects_to_delete.end();) {
		Object* object = *it;
		objects.remove(object);
		delete object;
		++it;
	}
	objects_to_delete.clear();
}

bool Game::compareZIndex(const Object* a, const Object* b) {
	return a->getZIndex() < b->getZIndex();
}
