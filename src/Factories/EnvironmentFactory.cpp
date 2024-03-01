#include "Components/Collider.h"
#include "Components/Renderers/SpriteRenderer.h"
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
}

EnvironmentFactory::~EnvironmentFactory() {
}


//___________________
// Public functions

void EnvironmentFactory::createOutside() {
	// Instantiate object
	Object* object = new Object(game, "outside");
	object->setZIndex(-2); // Behind the customers
	object->setAnchor(Vector2(0.f, 0.f));

	// Add components
	SpriteRenderer* renderer = new SpriteRenderer(
		game, *object, layout_path + "outside.png"
	);
}

void EnvironmentFactory::createDialogue() {
	// Instantiate object
	Object* object = new Object(game, "dialogue_box");
	object->setPosition(Vector2(0.f, game.getWindow().getSize().y));
	object->setAnchor(Vector2(0.f, 1.f));
	
	// Add components
	SpriteRenderer* renderer = new SpriteRenderer(
		game, *object, layout_path + "dialogue_box.png"
	);
}

void EnvironmentFactory::createStorage() {
	// Instantiate object
	Object* object = new Object(game, "storage");
	object->setPosition(Vector2(game.getWindow().getSize().x, 0.f));
	object->setAnchor(Vector2(1.f, 0.f));
	
	// Add components
	SpriteRenderer* renderer = new SpriteRenderer(
		game, *object, layout_path + "storage.png"
	);
	Collider* collider = new Collider(
		game, *object, renderer->getSize(), Layer::ItemDrop
	);
}

void EnvironmentFactory::createCounter() {
	// Instantiate object
	Object* object = new Object(game, "counter");
	object->setPosition(game.getWindow().getSize());
	object->setAnchor(Vector2(1.f, 1.f));

	// Add components
	SpriteRenderer* renderer = new SpriteRenderer(
		game, *object, layout_path + "counter.png"
	);

	createItemRegions(object);
}

void EnvironmentFactory::createItemRegions(Object* counter) {
	// Instantiate object
	Object* send_object = new Object(game, "send_region", counter);
	send_object->setLocalPosition(Vector2(-780.f, -180.f));
	
	// Add components
	SpriteRenderer* send_renderer = new SpriteRenderer(
		game, *send_object, layout_path + "send_region.png"
	);
	Collider* send_collider = new Collider(
		game, *send_object, send_renderer->getSize() * 1.1f, Layer::ItemDrop
	);

	// Instantiate object
	Object* receive_object = new Object(game, "receive_region", counter);
	receive_object->setLocalPosition(Vector2(-300.f, -180.f));

	// Add components
	SpriteRenderer* receive_renderer = new SpriteRenderer(
		game, *receive_object, layout_path + "receive_region.png"
	);
	Collider* receive_collider = new Collider(
		game, *receive_object,
		receive_renderer->getSize() * 1.1f,
		Layer::ItemReceive
	);
}