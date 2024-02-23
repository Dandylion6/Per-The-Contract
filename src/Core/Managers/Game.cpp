#include <memory>
#include <string>

#include <SFML/Graphics/RenderWindow.hpp>

#include "Core/Managers/Game.h"
#include "Core/Managers/Renderer.h"
#include "Core/Object.h"
#include "Factories/EnvironmentFactory.h"
#include "Factories/ItemFactory.h"
#include "Managers/CustomerManager.h"


//_______________
// Constructors

Game::Game(
	sf::RenderWindow& window, Renderer& renderer
) : window(window), renderer(renderer) {
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

Renderer& Game::getRenderer() const {
	return this->renderer;
}

Object* Game::getObject(std::string name) const {
	for (Object* object : objects) {
		if (object->getName() == name) return object;
	}
	return nullptr;
}

std::weak_ptr<CustomerManager> Game::getCustomerManager() const {
	return this->customer_manager;
}

std::weak_ptr<ItemFactory> Game::getItemFactory() const {
	return this->item_factory;
}


//___________________
// Public functions

void Game::update(float delta_time) {
	for (Object* object : objects) {
		object->update(delta_time);
	}
}

void Game::addObject(Object* object) {
	objects.push_back(object);
}

void Game::deleteObject(std::string name) {
	deleteObject(getObject(name));
}

void Game::deleteObject(Object* object) {
	objects.remove(object);
	delete object;
}


//____________________
// Private functions

void Game::CreateGame() {
	auto env_factory = std::make_unique<EnvironmentFactory>(*this);
	item_factory = std::make_shared<ItemFactory>(*this);
	customer_manager = std::make_shared<CustomerManager>(*this);
}
