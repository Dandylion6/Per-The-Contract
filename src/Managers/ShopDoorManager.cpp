#include "Components/Objects/ShopDoor.h"
#include "Components/Renderers/SpriteRenderer.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Managers/ShopDoorManager.h"

ShopDoorManager* ShopDoorManager::instance = nullptr;

//______________
// Constructors

ShopDoorManager::ShopDoorManager(Game& game) {
    if (instance != nullptr) {
        delete this;
        return;
    }
    instance = this;
    Object* door_object = new Object(game, "shop_door");
    new SpriteRenderer(game, *door_object, shop_door_path);
    shop_door = new ShopDoor(game, *door_object);
}

ShopDoorManager::~ShopDoorManager() {
}


//______________
// Getters

ShopDoorManager& ShopDoorManager::getInstance() {
    return *instance;
}

bool ShopDoorManager::isMoving() {
    return shop_door->isMoving();
}

void ShopDoorManager::closeDoor() {
    shop_door->closeDoor();
}

void ShopDoorManager::openDoor() {
    shop_door->openDoor();
}
