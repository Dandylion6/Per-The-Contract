#include <list>
#include <string>

#include "Core/Component.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Core/Utility/Vector2.h"


//_______________
// Constructors

Object::Object(Game& game, std::string name) : game(game) {
    this->name = name;
    game.addObject(this);
}

Object::Object(
    Game& game, std::string name, Object* parent
) : Object(game, name) {
    setParent(parent);
}

Object::~Object() {
    // Delete all the components of the object
    for (Component* component : components) {
        delete component;
    }
    // Clear the vector after deleting all components
    components.clear();
}


//__________
// Setters

void Object::setEnabled(bool enabled) {
    this->enabled = enabled;
}

void Object::setAnchor(Vector2 anchor) {
    this->anchor = Vector2::clamp(anchor, 0.f, 1.f);
}

void Object::setScale(Vector2 scale) {
    this->scale = scale;
    for (Object* child : children) {
        child->setScale(scale);
    }
}

void Object::setZIndex(int z_index) {
    if (parent != nullptr && z_index != parent->getZIndex()) return;
    this->z_index = z_index;
    game.resortObject(this);
    for (Object* child : children) {
        child->setZIndex(z_index);
        game.resortObject(child);
    }
}


//__________
// Getters

std::string Object::getName() const {
    return this->name;
}

bool Object::getEnabled() const {
    return this->enabled;
}

Object* Object::getParent() const {
    return this->parent;
}

Object* Object::getChild(std::string name) const {
    for (Object* child : children) {
        if (child->getName() == name) {
            return child;
        }
    }
    return nullptr;
}

std::list<Object*> Object::getChildren() const {
    return this->children;
}

Vector2 Object::getPosition() const {
    return this->position;
}

Vector2 Object::getLocalPosition() const {
    return this->local_position;
}

int Object::getZIndex() const {
    return this->z_index;
}

float Object::getRotation() const {
    return this->angle;
}

Vector2 Object::getAnchor() const {
    return this->anchor;
}

Vector2 Object::getScale() const {
    return this->scale;
}


//__________
// Setters

void Object::setParent(Object* parent) {
    if (this->parent != nullptr) { // Remove self from previous parent
        this->parent->children.remove(this);
    }
    this->parent = parent;
    if (parent == nullptr) {
        this->setLocalPosition(this->position);
        return;
    }

    this->parent->children.push_back(this); // Add self to new parent
    Vector2 new_local = this->position - this->parent->getPosition();
    this->setLocalPosition(new_local);
    this->setZIndex(this->parent->getZIndex()); // Make sure the child in in front of parent
}

void Object::setPosition(Vector2 position) {
    move(position - this->position);
}

void Object::setLocalPosition(Vector2 local_position) {
    this->local_position = local_position;
    if (parent == nullptr) {
        setPosition(local_position); // Set position directly
        return;
    }

    Vector2 parent_position = parent->getPosition();
    Vector2 global_position = parent_position + local_position;
    setPosition(global_position);
}

void Object::setRotation(float angle) {
    this->angle = angle;
    for (Object* child : children) {
        child->setRotation(angle);
    }
}


//___________________
// Public functions

void Object::move(Vector2 offset) {
    position += offset;

    for (Object* child : children) {
        child->move(offset);
    }
}

void Object::pushToFront() {
    game.resortObject(this); // Resorting inserts object to the back
}

void Object::addComponent(Component* component) {
    components.push_back(component);
}

void Object::update(float delta_time) {
    for (Component* component : components) {
        if (!component->getEnabled()) continue; // Don't update disabled
        component->update(delta_time);
    }
}
