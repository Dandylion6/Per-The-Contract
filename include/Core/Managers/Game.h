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
struct DealData;

enum class CustomerRequest
{
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

	const std::list<Object*>& getObjects() const;
	Object* getObject(std::string name) const;

	std::shared_ptr<DealData> getDealData() const;

	// Setters
	void setDealData(std::shared_ptr<DealData> deal_data);

	// Functions
	void resortObject(Object* object);
	void update(float delta_time);

	void startNextDeal();

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

	std::shared_ptr<DealData> deal_data = nullptr;

	// Functions
	void InstantiateGame();
	static bool compareZIndex(const Object* a, const Object* b);
};
