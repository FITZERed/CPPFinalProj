#include "MapManager.h"
#include "config.h"
#include <fstream>
#include <iostream>
#include <string>


void MapManager::LoadRandomMap(Player& player) {
    std::ifstream file("LevelLayouts/market1.txt"); // Example map file

    if (!file) {
        std::cerr << "Failed to load map file.\n";
        exit(1);
    }

    std::string line;
    int y = 0;
    while (std::getline(file, line) && y < MAP_HEIGHT) {
        if (line.length() < MAP_WIDTH) {
            std::cerr << "Warning: Line " << y << " is too short.\n";
        }

        for (int x = 0; x < MAP_WIDTH && x < line.length(); ++x) {
            char c = line[x];

            //  Get zone regardless of char replacement
            int zone = GetZoneFromChar(c);

            //  Fix: convert walkable chars to '.'
            if (c == ',' || c == '~' || c == '\'' || c == '.' || c == 'P') {
                map[y][x].character = '.';
            }
            else {
                map[y][x].character = c;
            }

            map[y][x].zone = zone;

            //  Assign player position
            if (c == 'P') {
                player.position = { (SHORT)x, (SHORT)y };
                player.currentZone = zone; //  Get from tile (now fixed)
            }
        }
        y++;
    }
}

bool MapManager::IsWalkable(char c) {
    return (c == '.' || c == ',' || c == '~' || c == '\''); //  Exclude shop numbers
}

int MapManager::GetZoneAt(int x, int y) {
    return map[y][x].zone;
}

char MapManager::GetCharAt(int x, int y) {
    return map[y][x].character;
}