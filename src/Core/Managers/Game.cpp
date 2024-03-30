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


//_______________
// Constructors

Game::Game(sf::RenderWindow& window) : window(window) {
	CreateGame();
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

DialogueManager& Game::getDialogueManager() const {
	return *this->dialogue_manager;
}

CustomerManager& Game::getCustomerManager() const {
	return *this->customer_manager;
}

EnvironmentFactory& Game::getEnvironmentFactory() const {
	return *this->environment_factory;
}

ItemFactory& Game::getItemFactory() const {
	return *this->item_factory;
}

StickerFactory& Game::getStickerFactory() const {
	return *this->sticker_factory;
}

CashFactory& Game::getCashFactory() const {
	return *this->cash_factory;
}

CustomerRequest Game::getCustomerRequest() const {
	return this->customer_request;
}

Item* Game::getItemNegotiating() const {
	return this->item_negotiating;
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


//__________
// Setters

void Game::setCustomerRequest(CustomerRequest customer_request) {
	this->customer_request = customer_request;
}

void Game::setItemNegotiating(Item* item_negotiating) {
	this->item_negotiating = item_negotiating;
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
	objects_to_delete.push_back(object);
}


//____________________
// Private functions

void Game::CreateGame() {
	cash_factory = std::make_unique<CashFactory>(*this);
	environment_factory = std::make_unique<EnvironmentFactory>(*this);
	item_factory = std::make_unique<ItemFactory>(*this);

	dialogue_manager = std::make_unique<DialogueManager>(*this);
	customer_manager = std::make_unique<CustomerManager>(*this);
	customer_manager->changeCustomer();

	sticker_factory = std::make_unique<StickerFactory>(*this);
	StickerPrinterFactory printer_factory(*this);

	Object* storage_object = getObject("storage");
	Vector2 size = storage_object->getComponent<SpriteRenderer>()->getSize();
	const std::vector<Cash*>& starting_cash = cash_factory->createCash(180u);
	for (Cash* cash : starting_cash) {
		Object& cash_object = cash->getObject();
		cash_object.setParent(storage_object);

		int x = utils::RandomGenerator::generateInt(40, 90);
		int y = utils::RandomGenerator::generateInt(40, 90);
		Vector2 position = Vector2(x - size.x, size.y - y);
		cash_object.setLocalPosition(position);
	}
}

bool Game::compareZIndex(const Object* a, const Object* b) {
	return a->getZIndex() < b->getZIndex();
}
