#pragma once

#include <cstdint>
#include <memory>

#include "SFML/Graphics/Color.hpp"

#include "Components/Collider.h"
#include "Components/Drag.h"
#include "Components/Renderers/TextRenderer.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Core/Utility/Vector2.h"
#include "Data/ItemData.h"
#include "Data/Role.h"

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
	uint16_t getCurrentPrice() const;
	uint16_t getLastPrice() const;

	// Setters
	void setOwned(bool is_owned_by_player);
	void setLatestOfferBy(Role offer_by);
	void setCurrentPrice(uint16_t current_price);

	// Functions
	void update(float delta_time) override;

private:
	// Constants
	float price_change_duration = 1.f;
	sf::Color profit_colour = sf::Color(85u, 204u, 66u, 255u);
	sf::Color loss_colour = sf::Color(230u, 49u, 87u, 255u);

	// Variables
	ItemData& data;
	TextRenderer* price_display = nullptr;
	TextRenderer* profit_display = nullptr;
	std::shared_ptr<Role> latest_offer_by = nullptr;
	bool is_owned_by_player = false;

	uint16_t buy_price = 0u;
	uint16_t last_price = 0u;
	uint16_t current_price = 0u;
	
	bool price_changing = false;
	float price_change_time = 0.f;

	// Functions
	void drop(Vector2& mouse_position) override;
	void updateRegionLock() override;
	void updateDroppableRegions() override;
	void updatePriceDisplay(float delta_time);
	void updateProfitDisplay(float delta_time);
	bool shouldDisplayProfit() const;
};