#include "MapManager.h"
#include "config.h"
#include <fstream>
#include <iostream>
#include <string>

void MapManager::LoadRandomMap(Player& player) {
    std::ifstream file("LevelLayouts/market1.txt"); // Use the first map

    if (!file) {
        std::cerr << "Failed to load map file.\n";
        exit(1);
    }

    std::string line;
    int y = 0;
    while (std::getline(file, line) && y < MAP_HEIGHT) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            char c = line[x];
            map[y][x].character = c;
            map[y][x].zone = GetZoneFromChar(c);

            if (c == 'P') {
                player.position = { (SHORT)x, (SHORT)y };
                player.currentZone = map[y][x].zone;
                map[y][x].character = '.'; // Clear player symbol
            }
        }
        y++;
    }
}

bool MapManager::IsWalkable(char c) {
    return (c == '.' || c == ',' || c == '~' || c == '\'' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5');
}

int MapManager::GetZoneAt(int x, int y) {
    return map[y][x].zone;
}