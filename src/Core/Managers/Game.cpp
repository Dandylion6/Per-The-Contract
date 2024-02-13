#include <list>
#include <new>
#include <string>

#include <SFML/Graphics/RenderWindow.hpp>

#include "Components/Renderer/SpriteRenderer.h"
#include "Core/Managers/Game.h"
#include "Core/Managers/Renderer.h"
#include "Core/Object.h"


//_______________
// Constructors

Game::Game(
	sf::RenderWindow& window, Renderer& renderer
) : window(window), renderer(renderer) {
	CreateGame();
}

Game::~Game() {
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


//___________________
// Public functions

void Game::update(float delta_time) {
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
	Object* object = new Object(*this, "test");
	SpriteRenderer* renderer = new SpriteRenderer(
		*this, *object, "assets/sprites/placeholder.png"
	);
	object->addComponent(renderer);
}
