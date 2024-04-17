#pragma once

#include <list>
#include <memory>
#include <string>

#include "SFML/Graphics/RenderWindow.hpp"


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

	void startNextDeal() const;
	void closeShop();

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
	Object* send_region = nullptr;
	Object* receive_region = nullptr;

	// Functions
	void InstantiateGame() const;
	static bool compareZIndex(const Object* a, const Object* b);
};
