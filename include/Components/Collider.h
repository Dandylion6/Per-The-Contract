#pragma once

#include "Core/Component.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Core/Utility/Bounds.h"
#include "Core/Utility/Vector2.h"

/// <summary>
/// Used to specify collision layer.
/// </summary>
enum Layer
{
	Default = 0,
	Item = 1,
	ItemDrop = 2,
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
	Collider* getClosestCollider(Layer layer_mask) const;

	// Functions
	void fitInto(Collider* target);
	void update(float delta_time) override;

private:
	// Variables
	static std::list<Collider*> global_colliders;
	Vector2 size = Vector2();
	Layer layer = Layer::Default;
};