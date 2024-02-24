#include "Components/Customer.h"
#include "Managers/CustomerManager.h"


//_______________
// Constructors

CustomerManager::CustomerManager(Game& game) : game(game) {
	createCustomer();
}

CustomerManager::~CustomerManager() {
}


//___________________
// Public functions

void CustomerManager::changeCustomer() {
}


//____________________
// Private functions

void CustomerManager::createCustomer() {
	// Build the objects
	Object* customer_object = new Object(game, "customer");
	Object* head_object = new Object(game, "head", customer_object);
	customer_object->setAnchor(Vector2(0.5f, 1.f));
	head_object->setAnchor(Vector2(0.5f, 1.f));
	customer_object->setPosition(Vector2(300.f, 800.f));

	// Add components
	torso_renderer = new SpriteRenderer(game, *customer_object);
	torso_renderer->setZIndex(-1);
	head_renderer = new SpriteRenderer(game, *head_object);
	customer = new Customer(game, *customer_object);
}

