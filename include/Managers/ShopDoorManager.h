#pragma once

#include <string>

#include "Components/Objects/ShopDoor.h"
#include "Core/Managers/Game.h"

class ShopDoorManager {
public:
    // Constructors
    ShopDoorManager(Game& game);
    virtual ~ShopDoorManager();

    // Getters
    static ShopDoorManager& getInstance();
    bool isMoving();

    // Functions
    void closeDoor();
    void openDoor();

private:
    // Constants
    const std::string shop_door_path = "assets/sprites/layout/shop_door.png";

    // Variables
    static ShopDoorManager* instance;
    ShopDoor* shop_door = nullptr;
};

