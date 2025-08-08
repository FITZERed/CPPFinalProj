#pragma once
#include <iostream>
#include <windows.h>
#include <vector>
#include "config.h"
#include "Player.h"

struct Tile {
    char character;
    int zone;
    int shopID = -1; // ID of the shop if this tile is a shop entrance

};

class MapManager {
public:
    Tile map[MAP_HEIGHT][MAP_WIDTH];
    //std::vector<Shop*> activeShops;
    std::vector<std::string> availableMaps;

    void LoadRandomMap(Player& player);
    int GetZoneAt(int x, int y);
    bool IsWalkable(char c);

    char GetCharAt(int x, int y);

    inline int GetZoneFromChar(char c) { // inline because it's only for parsing
        switch (c) {
        case '.': case 'P': return 0;
        case ',': return 1;
        case '~': return 2;
        case '\'': return 3;
        case ' ': return 4;
        default: return -1;
        }
    }
};