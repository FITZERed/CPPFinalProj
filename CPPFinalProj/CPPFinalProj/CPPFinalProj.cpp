#include <iostream>
#include <windows.h>
#include "MapManager.h"
#include "Player.h"
#include "GameState.h"
#include "InputHandler.h"
#include "MapRenderer.h"

int main() {
    GameState state = { 1, 1, 0, 300, 5, GameMode::Market };
    Player player;
    MapManager mapManager;

    mapManager.LoadRandomMap(player);

    while (true) {
        DrawMap(mapManager, player);

        SetCursor(65, 0);
        std::cout << "WEEK: " << state.currentWeek;
        SetCursor(65, 1);
        std::cout << "DAY: " << state.currentDay;
        SetCursor(65, 2);
        std::cout << "AP: " << state.actionPoints;
        SetCursor(65, 3);
        std::cout << "ZONE: " << player.currentZone;

        HandleInput(player, mapManager, state);

        Sleep(50);
    }
}