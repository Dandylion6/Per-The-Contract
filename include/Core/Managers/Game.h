#pragma once

#include <list>
#include <memory>
#include <string>

#include <SFML/Graphics/RenderWindow.hpp>

#include "Core/Managers/RenderManager.h"
#include "Factories/ItemFactory.h"

// Forward declerations
class Object;
class CustomerManager;

class Game
{
public:
	// Constructors
	Game(sf::RenderWindow& window, RenderManager& renderer);
	virtual ~Game();

	// Getters
	sf::RenderWindow& getWindow() const;
	RenderManager& getRenderManager() const;
	Object* getObject(std::string name) const;

	std::weak_ptr<CustomerManager> getCustomerManager() const;
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
	RenderManager& renderer;

	// Variables
	std::list<Object*> objects;
	std::shared_ptr<CustomerManager> customer_manager;
	std::shared_ptr<ItemFactory> item_factory;

	// Functions
	void CreateGame();
};
