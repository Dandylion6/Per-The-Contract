#pragma once

#include <memory>

#include "Components/Collider.h"
#include "Components/Drag.h"
#include "Components/Renderers/Renderer.h"
#include "Core/Utility/Vector2.h"
#include "Data/ItemData.h"
#include "Data/Role.h"

// Forward declerations
class Game;
class Object;

class Item : public Drag
{
public:
	// Constructors
	Item(
		Game& game,
		Object& object,
		Collider& collider,
		ItemData& data
	);
	virtual ~Item();

	// Getters
	ItemData& getData() const;
	bool getOwnedByPlayer() const;
	std::weak_ptr<Role> getLatestOfferBy() const;
	uint16_t getPrice() const;

	// Setters
	void setOwned(bool is_owned_by_player);
	void setLatestOfferBy(Role offer_by);
	void setPrice(uint16_t price);

private:
	// Variables
	ItemData& data;
	bool is_owned_by_player = false;
	std::shared_ptr<Role> latest_offer_by = nullptr;
	uint16_t price = 0u;

	// Functions
	void updateRegionLock() override;
	void updateDroppableRegions() override;
};