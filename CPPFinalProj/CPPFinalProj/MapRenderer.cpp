#include "MapRenderer.h"
#include <iostream>
#include <windows.h>
#include "config.h"
#include "MapManager.h"

void SetCursor(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void DrawMap(const MapManager& map, const Player& player) {
    SetCursor(0, 0);

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (player.position.X == x && player.position.Y == y) {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); // White
                std::cout << 'P';
            }
            else {
                char tile = map.map[y][x].character;
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8); // Grey
                std::cout << (tile == 'P' ? '.' : tile);
            }
        }
        std::cout << '\n';
    }
}