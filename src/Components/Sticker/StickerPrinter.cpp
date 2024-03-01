#include "Components/Sticker/StickerPrinter.h"
#include "Core/Component.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"


//_______________
// Constructors

StickerPrinter::StickerPrinter(
	Game& game, Object& object
) : Component(game, object) {
}

StickerPrinter::~StickerPrinter() {
}


//____________________
// Private functions

void StickerPrinter::update(float delta_time) {
}
