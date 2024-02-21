#pragma once

// Forward declerations
class Game;
class Object;

class Component
{
public:
	// Constructors
	Component(Game& game, Object& object);
	virtual ~Component();

	// Getters
	Object& getObject() const;
	bool getEnabled() const;

	// Setters
	void setEnabled(bool enabled);

	// Functions
	virtual void update(float delta_time) = 0;

protected:
	// References
	Game& game;
	Object& object;

	// Variables
	bool enabled = true;
};

