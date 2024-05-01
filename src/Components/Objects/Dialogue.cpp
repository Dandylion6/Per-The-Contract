#include "Components/Objects/Dialogue.h"
#include "Components/Renderers/TextRenderer.h"
#include "Core/Component.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Managers/DialogueManager.h"


//______________
// Contructors

Dialogue::Dialogue(
	Game& game, Object& object, TextRenderer& renderer
) : 
	Component(game, object), renderer(renderer)
{
}

Dialogue::~Dialogue() {
}


//___________________
// Public functions

void Dialogue::update(float delta_time) {
	if (!renderer.isTyping() && !continued_dialogue) {
		continued_dialogue = true;
		DialogueManager::getInstance().generateNext();
	}
}
