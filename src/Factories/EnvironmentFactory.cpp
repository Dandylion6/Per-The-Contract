#include "Components/Renderer/SpriteRenderer.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Core/Utility/Vector2.h"
#include "Factories/EnvironmentFactory.h"


//_______________
// Constructors

EnvironmentFactory::EnvironmentFactory(Game& game) : game(game) {
	createOutside();
	createDialogue();
	createStorage();
	createCounter();
	createItemRegions();
}

EnvironmentFactory::~EnvironmentFactory() {
}


//___________________
// Public functions

void EnvironmentFactory::createOutside() {
	Object* object = new Object(game, "outside");
	object->setAnchor(Vector2(0.f, 0.f));
	SpriteRenderer* renderer = new SpriteRenderer(
		game, *object, layout_path + "outside.png"
	);
}

void EnvironmentFactory::createDialogue() {
	Object* object = new Object(game, "dialogue_box");
	SpriteRenderer* renderer = new SpriteRenderer(
		game, *object, layout_path + "dialogue_box.png"
	);
	object->setPosition(Vector2(0.f, game.getWindow().getSize().y));
	object->setAnchor(Vector2(0.f, 1.f));
}

void EnvironmentFactory::createStorage() {
	Object* object = new Object(game, "dialogue_box");
	SpriteRenderer* renderer = new SpriteRenderer(
		game, *object, layout_path + "storage.png"
	);
	object->setPosition(Vector2(game.getWindow().getSize().x, 0.f));
	object->setAnchor(Vector2(1.f, 0.f));
}

void EnvironmentFactory::createCounter() {
	Object* object = new Object(game, "dialogue_box");
	SpriteRenderer* renderer = new SpriteRenderer(
		game, *object, layout_path + "counter.png"
	);
	object->setPosition(game.getWindow().getSize());
	object->setAnchor(Vector2(1.f, 1.f));
}

void EnvironmentFactory::createItemRegions() {
	Object* send_object = new Object(game, "send_region");
	SpriteRenderer* send_renderer = new SpriteRenderer(
		game, *send_object, layout_path + "send_region.png"
	);

	Object* receive_object = new Object(game, "receive_region");
	SpriteRenderer* receive_renderer = new SpriteRenderer(
		game, *receive_object, layout_path + "receive_region.png"
	);
}