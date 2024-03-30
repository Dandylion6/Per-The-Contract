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
class CustomerManager;
class DialogueManager;

enum class CustomerRequest
{
	None,
	Buying,
	Selling,
	Trading,
};

class Game
{
public:
	// Constructors
	Game(sf::RenderWindow& window);
	virtual ~Game();

	// Getters
	sf::RenderWindow& getWindow() const;
	DialogueManager& getDialogueManager() const;
	CustomerManager& getCustomerManager() const;

	EnvironmentFactory& getEnvironmentFactory() const;
	ItemFactory& getItemFactory() const;
	StickerFactory& getStickerFactory() const;
	CashFactory& getCashFactory() const;


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

	std::unique_ptr<DialogueManager> dialogue_manager;
	std::unique_ptr<CustomerManager> customer_manager;

	std::unique_ptr<EnvironmentFactory> environment_factory;
	std::unique_ptr<ItemFactory> item_factory;
	std::unique_ptr<StickerFactory> sticker_factory;
	std::unique_ptr<CashFactory> cash_factory;

	CustomerRequest customer_request = CustomerRequest::None;
	Item* item_negotiating = nullptr;

	// Functions
	void CreateGame();
	static bool compareZIndex(const Object* a, const Object* b);
};
