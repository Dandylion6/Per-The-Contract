#pragma once

#include <list>
#include <memory>
#include <string>

#include <SFML/Graphics/RenderWindow.hpp>

#include "Components/Item.h"
#include "Factories/CashFactory.h"
#include "Factories/EnvironmentFactory.h"
#include "Factories/ItemFactory.h"
#include "Factories/StickerFactory.h"

// Forward declerations
class Object;

enum class CustomerRequest
{
	None,
	Buying,
	Selling,
	Contract
};

class Game
{
public:
	// Constructors
	Game(sf::RenderWindow& window);
	virtual ~Game();

	// Getters
	sf::RenderWindow& getWindow() const;

	CustomerRequest getCustomerRequest() const;
	Item* getItemNegotiating() const;

	const std::list<Object*>& getObjects() const;
	Object* getObject(std::string name) const;

	// Setters
	void setCustomerRequest(CustomerRequest customer_request);
	void setItemNegotiating(Item* item_negotiating);

	// Functions
	void resortObject(Object* object);
	void update(float delta_time);

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

	CustomerRequest customer_request = CustomerRequest::None;
	Item* item_negotiating = nullptr;

	// Functions
	void InstantiateGame();
	static bool compareZIndex(const Object* a, const Object* b);
};
