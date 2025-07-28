#pragma once
#include <iostream>
#include <windows.h>
#include <vector>
#include "config.h"
#include "Player.h"

struct Tile {
    char character;
    int zone;
};

class MapManager {
public:
    Tile map[MAP_HEIGHT][MAP_WIDTH];
    //std::vector<Shop*> activeShops;
    std::vector<std::string> availableMaps;

    void LoadRandomMap(Player& player);
    int GetZoneAt(int x, int y);
    bool IsWalkable(char c);
};