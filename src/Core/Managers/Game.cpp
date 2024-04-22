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


//_______________
// Constructors

#include <iostream>

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
	objects_to_resort.clear();
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
	this->deal_data = deal_data;
}


//___________________
// Public functions

void Game::resortObject(Object* object) {
	objects_to_resort.push_back(object); // Add to be sorted after update
	for (Object* child : object->getChildren()) {
		objects_to_resort.push_back(child); // Children need to be sorted as well
	}
}

void Game::update(float delta_time) {
	resortObjects();

	// Update objects
	for (Object* object : objects) {
		if (object->getEnabled()) object->update(delta_time);
	}

	deleteObjects();

	// Update time of day
	since_last_hour += delta_time;
	if (since_last_hour >= game_hours_in_minutes * 60.f) {
		time_of_day = (time_of_day + 1) % 24;
		ContractManager::getInstance()->contractorArriveCheck();
		since_last_hour = 0.f;
	}
}

void Game::startNextDeal() const {
	if (deal_data != nullptr) return;

	// Make sure next deal can start
	if (send_region->getChildren().size() > 0u) return;
	if (receive_region->getChildren().size() > 0u) return;

	DialogueManager::getInstance().clearDialogue();
	CustomerManager::getInstance().changeCustomer();
	if (deal_data->request == CustomerRequest::Contract) {
		DialogueManager::getInstance().generateDialogue(Role::Merchant, "greeting_contractor");
	} else {
		DialogueManager::getInstance().generateDialogue(Role::Merchant, "greeting");
	}
}

void Game::closeShop() {
	deal_data.reset();
	startNextDeal(); // Start next deal if possible
}

void Game::addObject(Object* object) {
	if (object->getParent() != nullptr) {
		// If the object has a parent, add it after its parent's children in the objects vector
		auto parent_it = std::find(objects.begin(), objects.end(), object->getParent());
		if (parent_it != objects.end()) {
			// Found the parent, get its children
			std::list<Object*> children = (*parent_it)->getChildren();
			auto last_child_it = std::find_if(objects.begin(), objects.end(), [&](Object* obj) {
				return children.empty() || children.back()->getZIndex() < obj->getZIndex();
				}
			);
			objects.insert(last_child_it, object);
		} else {
			// Parent not found, add at the end
			objects.push_back(object);
		}
	} else {
		// No parent, add based on Z-index
		auto it = std::lower_bound(objects.begin(), objects.end(), object, compareZIndex);
		objects.insert(it, object);
	}
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

void Game::InstantiateGame() const {
	startNextDeal();

	Object* storage_object = getObject("storage");
	Vector2 size = storage_object->getComponent<SpriteRenderer>()->getSize();
	const std::vector<Cash*>& starting_cash = CashFactory::getInstance().createCash(240u);
	for (Cash* cash : starting_cash) {
		Object& cash_object = cash->getObject();
		cash_object.setParent(storage_object);

		Vector2 offset = utils::Random::randomRadius(50.f);
		Vector2 position = Vector2(offset.x - size.x, size.y - offset.y);
		cash_object.setLocalPosition(position + Vector2(80.f, -80.f));
	}
}

void Game::resortObjects() {
	for (Object* object : objects_to_resort) {
		auto it = std::find(objects.begin(), objects.end(), object);
		if (it != objects.end()) {
			objects.erase(it);
			addObject(object);
		}
	}
	objects_to_resort.clear();
}

void Game::deleteObjects() {
	for (auto it = objects_to_delete.begin(); it != objects_to_delete.end();) {
		Object* object = *it;
		objects.remove(object);
		objects_to_resort.remove(object); // Remove from re-sort if in list
		delete object;
		++it;
	}
	objects_to_delete.clear();
}

bool Game::compareZIndex(const Object* a, const Object* b) {
	return a->getZIndex() < b->getZIndex();
}
