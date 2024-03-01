#pragma once

#include "Components/Collider.h"
#include "Components/Sticker/StickerPrinter.h"
#include "Core/Component.h"

// Forward declerations
class Game;
class Object;

class PrinterKey : Component
{
public:
	// Constructors
	PrinterKey(
		Game& game, Object& object,
		StickerPrinter& printer, Collider& collider,
		uint8_t output
	);
	virtual ~PrinterKey();

	// Functions
	void update(float delta_time) override;

private:
	// References
	StickerPrinter& printer;
	Collider& collider;

	// Variables
	uint8_t output;

	// Functions
	bool keyPressed() const;
};

