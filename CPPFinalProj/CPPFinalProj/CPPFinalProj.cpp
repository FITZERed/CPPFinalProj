#include <iostream>
#include <windows.h>
#include "MapManager.h"
#include "Player.h"
#include "GameState.h"
#include "InputHandler.h"
#include "MapRenderer.h"
#include "ConsoleUtils.h"
#include "ShopManager.h"

int main() {
    GameState state = { 1, 1, 0, 300, 5, GameMode::Market };
    Player player;
    MapManager mapManager;
    ShopManager shopManager;

    shopManager.LoadShops();
    mapManager.LoadRandomMap(player);

    while (true) {
        if (state.mode == GameMode::Market) {
            DrawMap(mapManager, player);
        }

        SetCursor(65, 0);
        std::cout << "WEEK: " << state.currentWeek;
        SetCursor(65, 1);
        std::cout << "DAY: " << state.currentDay;
        SetCursor(65, 2);
        std::cout << "AP: " << state.actionPoints;
        SetCursor(65, 3);
        std::cout << "ZONE: " << player.currentZone;
        SetCursor(65, 4);
        std::cout << "MODE: " << (state.mode == GameMode::Market ? "Market" : "Shop");

        HandleInput(player, mapManager, state, shopManager);

        Sleep(50);
    }
}