#include <string>
#include "Core/Object.h"


//_______________
// Constructors

Object::Object(Game& game, std::string name) : game(game) {
    this->name = name;
}

Object::~Object() {
}


//__________
// Getters

std::string Object::getName() const {
    return this->name;
}
