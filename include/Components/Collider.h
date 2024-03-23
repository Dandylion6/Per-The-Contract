#pragma once

#include <list>

#include "Core/Component.h"
#include "Core/Utility/Bounds.h"
#include "Core/Utility/Vector2.h"

// Forward declerations
class Game;
class Object;

enum Layer
{
	Default,
	Dragable,
	ItemDrop,
	ItemReceive
};

class Collider : public Component
{
public:
	// Constructors
	Collider(Game& game, Object& object, Vector2 size);
	Collider(Game& game, Object& object, Vector2 size, Layer layer);
	virtual ~Collider();

	// Getters
	Vector2 getSize() const;
	Layer getLayer() const;
	Bounds getLocalBounds() const;
	Bounds getBounds() const;

	Collider* getMostOverlapping() const;
	Collider* getMostOverlapping(Layer layer_mask) const;
	static Collider* getColliderWithLayer(Layer target_layer);

	// Functions
	bool pointHits(Vector2 point);
	bool pointHits(Vector2 point, Layer layer_mask);
	void fitInto(Collider* target);
	void update(float delta_time) override;

private:
	// Variables
	static std::list<Collider*> global_colliders;
	Vector2 size = Vector2();
	Layer layer = Layer::Default;
};