#include "MapRenderer.h"
#include <iostream>
#include <windows.h>
#include "config.h"
#include "MapManager.h"
#include "ConsoleUtils.h"


void DrawMap(const MapManager& mapManager, const Player& player) {
    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            SetCursor(x, y);

            if (x == player.position.X && y == player.position.Y) {
                SetColor(14); // Yellow for player
                std::cout << '@';
                continue;
            }

            const Tile& tile = mapManager.map[y][x];

            //  Visualize zone with color
            switch (tile.zone) {
            case 0: SetColor(8); break;  // Gray
            case 1: SetColor(10); break; // Green
            case 2: SetColor(11); break; // Cyan
            case 3: SetColor(13); break; // Magenta
            case 4: SetColor(12); break; // Red
            default: SetColor(7); break; // White
            }
            if (tile.character == '.' || tile.character == ',' || tile.character == '~' || tile.character == '\'') {
                std::cout << '.';
            }
            else {
                std::cout << tile.character;
            }
        }
    }

    SetColor(7); // Reset color
}