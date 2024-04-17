#include <list>
#include <memory>
#include <string>

#include <SFML/Graphics/RenderWindow.hpp>

#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Core/Utility/RandomGenerator.h"
#include "Factories/EnvironmentFactory.h"
#include "Factories/ItemFactory.h"
#include "Factories/StickerPrinterFactory.h"
#include "Managers/CustomerManager.h"
#include "Managers/DialogueManager.h"
#include "Data/DealData.h"
#include "Factories/DealClosureFactory.h"


//_______________
// Constructors

Game::Game(sf::RenderWindow& window) : window(window) {
	new EnvironmentFactory(*this);
	new DialogueManager(*this);

	new CashFactory(*this);
	new ItemFactory(*this);

	new CustomerManager(*this);
	new StickerFactory(*this);
	new StickerPrinterFactory(*this);
	new DealClosureFactory(*this);

	send_region = getObject("send_region");
	receive_region = getObject("receive_region");
	InstantiateGame();
}

Game::~Game() {
	// Deleting because it is good practice, but would be done anyway on exit
	for (auto it = objects.begin(); it != objects.end();) {
		delete *it;
		it = objects.erase(it);
	}
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
}

void Game::update(float delta_time) {
	// Update objects
	for (Object* object : objects) {
		object->update(delta_time);
	}

	// Re-sort objects
	for (Object* object : objects_to_resort) {
		objects.remove(object);
		addObject(object); // This will re-sort the object
	}
	objects_to_resort.clear();

	// Delete objects
	for (auto it = objects_to_delete.begin(); it != objects_to_delete.end();) {
		Object* object = *it;
		objects.remove(object);
		objects_to_resort.remove(object); // Remove from re-sort if in list
		delete object;
		++it;
	}
	objects_to_delete.clear();
}

void Game::startNextDeal() const {
	if (deal_data != nullptr) return;

	// Make sure next deal can start
	if (send_region->getChildren().size() > 0u) return;
	if (receive_region->getChildren().size() > 0u) return;

	DialogueManager::getInstance().generateDialogue(Role::Merchant, "greeting");
	CustomerManager::getInstance().changeCustomer();
}

void Game::closeShop() {
	deal_data.reset();
}

void Game::addObject(Object* object) {
	auto it = std::upper_bound(
		objects.begin(), objects.end(), object, compareZIndex
	); // Find insert point based on z index (inserts to the back)
	objects.insert(it, object);
}

void Game::deleteObject(std::string name) {
	deleteObject(getObject(name));
}

void Game::deleteObject(Object* object) {
	object->setParent(nullptr);
	objects_to_delete.push_back(object);
}


//____________________
// Private functions

void Game::InstantiateGame() {
	startNextDeal();

	Object* storage_object = getObject("storage");
	Vector2 size = storage_object->getComponent<SpriteRenderer>()->getSize();
	const std::vector<Cash*>& starting_cash = CashFactory::getInstance().createCash(180u);
	for (Cash* cash : starting_cash) {
		Object& cash_object = cash->getObject();
		cash_object.setParent(storage_object);

		int x = utils::Random::generateInt(40, 120);
		int y = utils::Random::generateInt(40, 120);
		Vector2 position = Vector2(x - size.x, size.y - y);
		cash_object.setLocalPosition(position);
	}
}

bool Game::compareZIndex(const Object* a, const Object* b) {
	return a->getZIndex() < b->getZIndex();
}
