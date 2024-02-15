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
	Bounds getLocalBounds() const;
	Bounds getBounds() const;

	// Functions
	void update(float delta_time) override;

private:
	// Variables
	Vector2 size = Vector2();
	Layer layer = Layer::Default;
};