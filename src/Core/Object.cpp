#include <string>

#include "Core/Component.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Core/Utility/Vector2.h"


//_______________
// Constructors

Object::Object(Game& game, std::string name) : game(game) {
    this->name = name;
    this->parent = nullptr;
    game.addObject(this);
}

Object::Object(
    Game& game, std::string name, Object* parent
) : game(game) {
    this->name = name;
    game.addObject(this);
    setParent(parent);
}

Object::~Object() {
    for (Component* component : components) {
        delete component;
    }
}


//__________
// Setters

void Object::setAnchor(Vector2 anchor) {
    *this->anchor = anchor;
}


//__________
// Getters

std::string Object::getName() const {
    return this->name;
}

Object* Object::getParent() const {
    return this->parent;
}

Vector2 Object::getPosition() const {
    return *this->position;
}

Vector2 Object::getLocalPosition() const {
    return *this->local_position;
}

Vector2 Object::getAnchor() const {
    return *this->anchor;
}


//__________
// Setters

void Object::setParent(Object* parent) {
    if (this->parent != nullptr) { // Remove child from previous parent
        this->parent->children.remove(this);
    }
    this->parent = parent;
    Vector2 new_local = *this->position - parent->getLocalPosition();
    this->setLocalPosition(new_local);
}

void Object::setPosition(Vector2 position) {
    Vector2 difference = position - *this->position;
    move(difference);
}

void Object::setLocalPosition(Vector2 position) {
    Vector2 difference = position - *this->local_position;
    move(difference);
}


//___________________
// Public functions

void Object::move(Vector2 offset) {
    *position = *position + offset;
    *local_position = *position + offset;

    for (Object* child : children) {
        child->move(offset);
    }
}

void Object::addComponent(Component* component) {
    components.push_back(component);
}

void Object::update(float delta_time) {
    for (Component* component : components) {
        component->update(delta_time);
    }
}
