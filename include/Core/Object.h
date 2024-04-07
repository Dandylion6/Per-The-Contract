#pragma once

#include <list>
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
	void setScale(Vector2 scale);
	void setZIndex(int z_index);

	// Getters
	std::string getName() const;
	Object* getParent() const;
	Object* getChild(std::string name) const;
	std::list<Object*> getChildren() const;

	Vector2 getPosition() const;
	Vector2 getLocalPosition() const;
	int getZIndex() const;
	float getRotation() const;

	Vector2 getAnchor() const;
	Vector2 getScale() const;

	template <class T>
	T* getComponent() const {
		for (Component* component : components) {
			if (T* result = dynamic_cast<T*>(component)) {
				return result;
			}
		}
		return nullptr;
	}

	// Setters
	void setParent(Object* parent);
	void setPosition(Vector2 position);
	void setLocalPosition(Vector2 local_position);
	void setRotation(float angle);

	// Functions
	void move(Vector2 offset);
	void pushToFront();
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

	Vector2 position = Vector2();
	Vector2 local_position = Vector2();
	int z_index = 0;
	float angle = 0.f;

	Vector2 anchor = Vector2(0.5f, 0.5f);
	Vector2 scale = Vector2(1.f, 1.f);
};