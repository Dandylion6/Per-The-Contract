#pragma once

#include "Components/Collider.h"
#include "Components/Drag.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Core/Utility/Vector2.h"

class Contract : public Drag {
public:
	// Constructors
	Contract(Game& game, Object& object, Collider& collider);
	virtual ~Contract();

	// functions
	bool isCompleted();

private:
	// Functions
	void drop(Vector2& mouse_position) override;
	void updateRegionLock() override;
	void updateDroppableRegions() override;
};