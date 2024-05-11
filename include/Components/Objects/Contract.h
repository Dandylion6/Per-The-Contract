#pragma once

#include <cstdint>
#include <string>

#include "Components/Collider.h"
#include "Components/Drag.h"
#include "Components/Renderers/TextRenderer.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Core/Utility/Bounds.h"
#include "Core/Utility/Vector2.h"
#include "Data/OrderType.h"

class Contract : public Drag {
public:
	// Constructors
	Contract(
		Game& game,
		Object& object,
		Collider& collider, 
		uint8_t hours,
		OrderType order_type
	);
	virtual ~Contract();

	// Getters
	uint8_t getHours() const;
	OrderType getOrderType() const;

	// functions
	virtual void update(float delta_time) override;

private:

	// Variables
	uint8_t hours = 0u;
	TextRenderer* details_renderer = nullptr;
	Bounds details_bounds;
	OrderType order_type;

	// Functions
	void drop(Vector2& mouse_position) override;
	void updateRegionLock() override;
	void updateDroppableRegions() override;
	std::string getText();
};