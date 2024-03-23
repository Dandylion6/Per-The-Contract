#include <list>
#include <memory>
#include <string>

#include <SFML/Graphics/RenderWindow.hpp>

#include "Core/Managers/Game.h"
#include "Core/Object.h"
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

std::weak_ptr<CustomerManager> Game::getCustomerManager() const {
	return this->customer_manager;
}

std::weak_ptr<ItemFactory> Game::getItemFactory() const {
	return this->item_factory;
}

std::weak_ptr<StickerFactory> Game::getStickerFactory() const {
	return this->sticker_factory;
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
	EnvironmentFactory environment_factory(*this);

	item_factory = std::make_shared<ItemFactory>(*this);
	customer_manager = std::make_shared<CustomerManager>(*this);
	customer_manager->changeCustomer();

	item_factory->createItem("test_item");
	sticker_factory = std::make_shared<StickerFactory>(*this);
	StickerPrinterFactory printer_factory(*this);
}

bool Game::compareZIndex(const Object* a, const Object* b) {
	return a->getZIndex() < b->getZIndex();
}
