#pragma once

#include <string>

#include "Core/Utility/Vector2.h"

// Forward declerations
class Game;

class DealClosureFactory {
public:
	// Constructors
	DealClosureFactory(Game& game);
	virtual ~DealClosureFactory();

private:
	// Constants
	const std::string deal_closure_directory = "assets/sprites/objects/deal_closure/";
	const std::string panel_path = deal_closure_directory + "deal_closure_panel.png";
	const std::string accept_button_path = deal_closure_directory + "accept_deal_button.png";
	const std::string decline_button_path = deal_closure_directory + "decline_deal_button.png";

	const Vector2 panel_position = Vector2(320.f, 740.f);
	const Vector2 decline_button_position = Vector2(-30.f, 0.f);
	const Vector2 accept_button_position = Vector2(30.f, 0.f);

	// Functions
	void createClosureObjects(Game& game) const;
};

