#include <iostream>
#include <windows.h>
#include "MapManager.h"
#include "Player.h"
#include "GameState.h"
#include "InputHandler.h"
#include "MapRenderer.h"
#include "ConsoleUtils.h"
#include "ShopManager.h"
#include "ShopRenderer.h"
#include "ShopUI.h"
#include "config.h"
#include <thread>

int main() {
    GameState state = { 1, 1, 0, STARTING_MONEY, STARTING_ACTION_POINTS, GameMode::Market };
    Player player(STARTING_MONEY);
    MapManager mapManager;
    ShopManager shopManager;

    shopManager.LoadShops();
    mapManager.LoadRandomMap(player);

    while (true) {
        // Render world or shop screen
        if (state.mode == GameMode::Market) {
            DrawMap(mapManager, player);
        }
        else { // Shop mode
            // Draw shop art and static info
            if (state.currentShop) DrawShop(state.currentShop, player);
            else DrawShop(nullptr, player);

            // Draw interactive menu
            if (state.currentShop) ShopUI::Render(state.currentShop, player);
            else ShopUI::Render(nullptr, player);
        }

        // HUD below the map (always visible)
        int hudY = MAP_HEIGHT + 1;
        SetCursor(0, hudY);
        std::cout << "WEEK: " << state.currentWeek << "  "
            << "DAY: " << state.currentDay << "  "
            << "AP: " << state.actionPoints << "  "
            << "ZONE: " << player.currentZone << "  "
            << "MONEY: " << player.inventory.money << "        ";

        SetCursor(0, hudY + 1);
        std::cout << "MODE: " << (state.mode == GameMode::Market ? "Market" : "Shop") << "        ";

        // Let the input handler take care of user input (market movement or shop UI)
        HandleInput(player, mapManager, state, shopManager);

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    return 0;
}