#pragma once
#include <windows.h>
//#include "Inventory.h"

class MapManager;
class GameState;

class Player {
public:
    COORD position;
    int currentZone = 0;

    void Move(int dx, int dy, MapManager& mapManager, GameState& gameState);
};
