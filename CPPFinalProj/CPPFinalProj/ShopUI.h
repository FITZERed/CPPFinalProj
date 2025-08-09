#pragma once
#include "Shop.h"
#include "Player.h"
#include "GameState.h"

// Shop UI module
namespace ShopUI {
    // Call when entering a shop to initialize internal UI state
    void EnterShop(Shop* shop, Player* player, GameState* state);

    // Render the interactive shop menu (draws into the menu area at MAP_WIDTH+2)
    void Render(Shop* shop, const Player& player);

    // Handle a key press while in Shop mode. Expects caller to pass the raw key char from _getch().
    // Returns true if the UI handled the key and wants the main loop to continue, false otherwise.
    void HandleInput(char key, Shop* shop, Player& player, GameState& state);
}