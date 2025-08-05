#include "MapRenderer.h"
#include <iostream>
#include <windows.h>
#include "config.h"
#include "MapManager.h"
#include "ConsoleUtils.h"


void DrawMap(const MapManager& mapManager, const Player& player) {
    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            const Tile& tile = mapManager.map[y][x];

            SetCursor(x, y);

            // Convert tile visuals
            char displayChar = tile.character;
            if (tile.character == '~' || tile.character == ',' || tile.character == '\'') {
                displayChar = '.';  
            }

            // Player rendering
            if (player.position.X == x && player.position.Y == y) {
                std::cout << '@';
            }
            else {
                std::cout << displayChar;
            }
        }
    }
}