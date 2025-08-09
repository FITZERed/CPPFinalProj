#pragma once
#include <windows.h>
#include "Inventory.h"

class MapManager;
class GameState;

class Player {
public:
    COORD position;
    Inventory inventory;
    int currentZone = 0;

    Player(int startingMoney);

    void Move(int dx, int dy, MapManager& mapManager, GameState& gameState);
};
