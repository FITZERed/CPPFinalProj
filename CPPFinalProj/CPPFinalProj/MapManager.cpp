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

        for (int x = 0; x < MAP_WIDTH && x < (int)line.length(); ++x) {
            char c = line[x];

            int zone = GetZoneFromChar(c);
            map[y][x].zone = zone;
            map[y][x].shopID = -1; // default: no shop here

            if (c == 'P') {
                // Player start position
                player.position = { (SHORT)x, (SHORT)y };
                player.currentZone = zone;
                map[y][x].character = '.'; // clear 'P' from map
                continue; // done with this tile
            }

            if (c >= '1' && c <= '5') {
                int shopIndex = c - '1';
                map[y][x].shopID = shopIndex;
                map[y][x].character = c;
            }
            else if (c == ',' || c == '~' || c == '\'' || c == '.') {
                map[y][x].character = '.'; // normalize walkables
            }
            else {
                map[y][x].character = c; // keep whatever it is
            }
        }
        ++y;
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