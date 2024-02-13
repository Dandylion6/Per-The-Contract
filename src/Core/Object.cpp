#include <string>
#include <vector>

#include "Core/Component.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"


//_______________
// Constructors

Object::Object(Game& game, std::string name) : game(game) {
    this->name = name;
    game.addObject(this);
}

Object::~Object() {
}


//__________
// Getters

std::string Object::getName() const {
    return this->name;
}


//___________________
// Public functions

void Object::addComponent(Component* component) {
    components.push_back(component);
}

void Object::update(float delta_time) {
    for (Component* component : components) {
        component->update(delta_time);
    }
}
