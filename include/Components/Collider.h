#pragma once

#include <list>
#include <string>
#include <vector>

#include "Core/Component.h"
#include "Core/Utility/Bounds.h"
#include "Core/Utility/Vector2.h"

// Forward declerations
class Game;
class Object;

class Collider : public Component
{
public:
	// Constructors
	Collider(Game& game, Object& object, Vector2 size);
	virtual ~Collider();

	// Getters
	Vector2 getSize() const;
	Bounds getLocalBounds() const;
	Bounds getBounds() const;

	Collider* getOverlapping() const;
	Collider* getMostOverlapping(std::vector<Collider*> colliders) const;
	static Collider* getCollider(std::string name);

	// Functions
	bool pointHits(Vector2 point);
	void fitInto(Collider* target);
	void update(float delta_time) override;

private:
	// Variables
	static std::list<Collider*> global_colliders;
	Vector2 size = Vector2();
};