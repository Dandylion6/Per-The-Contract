#pragma once

#include <list>
#include <memory>
#include <string>
#include <vector>

#include "Core/Utility/Vector2.h"

// Forward decleration
class Game;
class Component;

class Object
{
public:
	// Constructors
	Object(Game& game, std::string name);
	Object(Game& game, std::string name, Object* parent);
	virtual ~Object();

	// Setters
	void setAnchor(Vector2 anchor);

	// Getters
	std::string getName() const;
	Object* getParent() const;
	Vector2 getPosition() const;
	Vector2 getLocalPosition() const;
	Vector2 getAnchor() const;

	// Setters
	void setParent(Object* parent);
	void setPosition(Vector2 position);
	void setLocalPosition(Vector2 position);

	// Functions
	void move(Vector2 offset);
	void addComponent(Component* component);
	void update(float delta_time);

private:
	// References
	Game& game;

	// Variables
	std::string name;
	Object* parent = nullptr;
	std::vector<Component*> components;
	std::list<Object*> children;

	std::unique_ptr<Vector2> position = std::make_unique<Vector2>();
	std::unique_ptr<Vector2> local_position = std::make_unique<Vector2>();
	std::unique_ptr<Vector2> anchor = std::make_unique<Vector2>(
		Vector2(0.5f, 0.5f)
	);
};