#pragma once

#include <list>
#include <memory>
#include <string>

#include <SFML/Graphics/RenderWindow.hpp>

#include "Core/Managers/Renderer.h"
#include "Core/Object.h"
#include "Factories/ItemFactory.h"

class Game
{
public:
	// Constructors
	Game(sf::RenderWindow& window, Renderer& renderer);
	virtual ~Game();

	// Getters
	sf::RenderWindow& getWindow() const;
	Renderer& getRenderer() const;
	Object* getObject(std::string name) const;

	std::weak_ptr<ItemFactory> getItemFactory() const;

	// Functions
	void update(float delta_time);
	
	// Object functions
	void addObject(Object* object);
	void deleteObject(std::string name);
	void deleteObject(Object* object);

private:
	// References
	sf::RenderWindow& window;
	Renderer& renderer;

	// Variables
	std::list<Object*> objects;
	std::shared_ptr<ItemFactory> item_factory;

	// Functions
	void CreateGame();
};
