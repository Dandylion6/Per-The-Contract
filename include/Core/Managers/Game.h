#pragma once

#include <list>
#include <string>

#include <SFML/Graphics/RenderWindow.hpp>

#include "Core/Managers/Renderer.h"
#include "Core/Object.h"

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

	// Functions
	void CreateGame();
};
