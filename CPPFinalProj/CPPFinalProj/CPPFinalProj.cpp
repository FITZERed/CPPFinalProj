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
    GameState state = { 1, 1, 0, STARTING_MONEY, STARTING_ACTION_POINTS, GameMode::Market };
    Player player;
    MapManager mapManager;
    ShopManager shopManager;

    shopManager.LoadShops();
    mapManager.LoadRandomMap(player);

    while (true) {
        DrawMap(mapManager, player); // always draw map in both modes

        // HUD position: one line gap below map
        int hudY = MAP_HEIGHT + 1;
        SetCursor(0, hudY);
        std::cout << "WEEK: " << state.currentWeek << "  "
            << "DAY: " << state.currentDay << "  "
            << "AP: " << state.actionPoints << "  "
            << "ZONE: " << player.currentZone << "  "
            << "MONEY: " << player.inventory.money;

        SetCursor(0, hudY + 1);
        std::cout << "MODE: " << (state.mode == GameMode::Market ? "Market" : "Shop");

        HandleInput(player, mapManager, state, shopManager);
        Sleep(50);
    }
}