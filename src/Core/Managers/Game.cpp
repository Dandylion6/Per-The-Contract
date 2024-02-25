#include <list>
#include <memory>
#include <string>

#include <SFML/Graphics/RenderWindow.hpp>

#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Factories/EnvironmentFactory.h"
#include "Factories/ItemFactory.h"
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
	for (Object* object : objects) {
		object->update(delta_time);
	}

	// Re-sort objects
	for (Object* object : objects_to_resort) {
		objects.remove(object);
		addObject(object); // This will re-sort the object
	}
	objects_to_resort.clear();
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
	objects.remove(object);
	objects_to_resort.remove_if(
		[object](Object* obj) {
			return obj == object;
		}
	); // Remove from re-sort if in list
	delete object;
}


//____________________
// Private functions

void Game::CreateGame() {
	auto env_factory = std::make_unique<EnvironmentFactory>(*this);
	item_factory = std::make_shared<ItemFactory>(*this);
	customer_manager = std::make_shared<CustomerManager>(*this);
	customer_manager->changeCustomer();

	Item* item1 = item_factory->createItem("test_item", getObject("receive_region"));
	item1->getObject().setLocalPosition(Vector2(-100.f, -100.f));
}

bool Game::compareZIndex(const Object* a, const Object* b) {
	return a->getZIndex() < b->getZIndex();
}
