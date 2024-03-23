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
	std::weak_ptr<CustomerManager> getCustomerManager() const;
	std::weak_ptr<ItemFactory> getItemFactory() const;
	std::weak_ptr<StickerFactory> getStickerFactory() const;
	
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

	std::shared_ptr<CustomerManager> customer_manager;
	std::shared_ptr<ItemFactory> item_factory;
	std::shared_ptr<StickerFactory> sticker_factory;

	// Functions
	void CreateGame();
	static bool compareZIndex(const Object* a, const Object* b);
};
