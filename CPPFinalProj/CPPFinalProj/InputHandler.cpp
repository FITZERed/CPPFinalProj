#include "InputHandler.h"
#include "Player.h"
#include <conio.h>
#include "MapManager.h"
#include "ShopManager.h"

bool IsAdjacentToShop(const Player& player, const MapManager& map, int& shopID) {
    COORD pos = player.position;
    COORD offsets[4] = { {1,0}, {-1,0}, {0,1}, {0,-1} };

    for (const auto& offset : offsets) {
        int x = pos.X + offset.X;
        int y = pos.Y + offset.Y;
        if (x < 0 || y < 0 || x >= MAP_WIDTH || y >= MAP_HEIGHT)
            continue;

        int id = map.map[y][x].shopID;
        if (id >= 0) {
            shopID = id;
            return true;
        }
    }
    return false;
}

void HandleInput(Player& player, MapManager& map, GameState& gameState, ShopManager& shopManager) {
    if (!_kbhit()) return;

    char key = _getch();
    switch (key) {
    case 'w': player.Move(0, -1, map, gameState); break;
    case 's': player.Move(0, 1, map, gameState); break;
    case 'a': player.Move(-1, 0, map, gameState); break;
    case 'd': player.Move(1, 0, map, gameState); break;
    case 'e': {
        int shopID = -1;
        if (IsAdjacentToShop(player, map, shopID)) {
            Shop* shop = shopManager.GetShopByID(shopID);
            if (shop) {
                gameState.mode = GameMode::Shop;
                gameState.currentShop = shop;
                system("cls");
                std::cout << "Opened Shop: " << shop->name << "\n";
            }
        }
        break;
    }
    }
}