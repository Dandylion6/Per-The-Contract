#include <cmath>

#include "Components/Cash.h"
#include "Components/Collider.h"
#include "Components/Renderers/SpriteRenderer.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Factories/CashFactory.h"


CashFactory* CashFactory::instance = nullptr;

//_______________
// Constructors

CashFactory::CashFactory(Game& game) : game(game) {
	if (instance != nullptr) {
		delete this;
		return;
	}
	instance = this;
}

CashFactory::~CashFactory() {
}


//__________
// Getters

CashFactory& CashFactory::getInstance() {
	return *instance;
}


//___________________
// Public functions

std::vector<Cash*> CashFactory::createCash(uint16_t value) {
	std::vector<Cash*> cash_objects;
	std::vector<uint8_t> split = splitValue(value);

	for (uint8_t cash_value : split) {
		std::string name = std::to_string(cash_value) + "_cash";
		Object* object = new Object(game, name);

		SpriteRenderer* renderer = new SpriteRenderer(
			game, *object, sprite_directory + name + ".png"
		);
		Collider* collider = new Collider(
			game, *object, renderer->getSize()
		);
		cash_objects.push_back(
			new Cash(game, *object, *collider, cash_value)
		);
	}
	return cash_objects;
}


//____________________
// Private functions

std::vector<uint8_t> CashFactory::splitValue(uint16_t value) {
	std::vector<uint8_t> result;
	for (uint8_t cash_value : cash_values) {
		uint8_t amount = value / cash_value; // How many does this fit into?
		if (amount == 0u) continue;
		value -= cash_value * amount; // Remove splitted value
		for (uint8_t i = 0u; i < amount; i++) {
			result.push_back(cash_value);
		}
	}
	return result;
}