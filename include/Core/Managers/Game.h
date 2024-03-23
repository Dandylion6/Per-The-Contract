#pragma once

#include <list>
#include <memory>
#include <string>

#include <SFML/Graphics/RenderWindow.hpp>

#include "Factories/ItemFactory.h"
#include "Factories/StickerFactory.h"

// Forward declerations
class Object;
class CustomerManager;

class Game
{
public:
	// Constructors
	Game(sf::RenderWindow& window);
	virtual ~Game();

	// Getters
	sf::RenderWindow& getWindow() const;
	CustomerManager& getCustomerManager() const;
	ItemFactory& getItemFactory() const;
	StickerFactory& getStickerFactory() const;
	
	const std::list<Object*>& getObjects() const;
	Object* getObject(std::string name) const;

	// Functions
	void resortObject(Object* object);
	void update(float delta_time);

	// Object functions
	void addObject(Object* object);
	void deleteObject(std::string name);
	void deleteObject(Object* object);

private:
	// References
	sf::RenderWindow& window;

	// Variables
	std::list<Object*> objects;
	std::list<Object*> objects_to_resort;
	std::list<Object*> objects_to_delete;

	std::unique_ptr<CustomerManager> customer_manager;
	std::unique_ptr<ItemFactory> item_factory;
	std::unique_ptr<StickerFactory> sticker_factory;

	// Functions
	void CreateGame();
	static bool compareZIndex(const Object* a, const Object* b);
};
