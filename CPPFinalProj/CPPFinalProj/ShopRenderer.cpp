#include "ShopRenderer.h"
#include "ConsoleUtils.h"
#include "config.h"
#include <iostream>
#include <string>
#include <vector>

// Keep same SHOPKEEPER_ART as before
static const std::vector<std::string> SHOPKEEPER_ART = {
    "      ______________________________      ",
    "     /                              \\     ",
    "    /   [ Shop of Fine Wares ]       \\    ",
    "   |   ____________________________   |   ",
    "   |  |                            |  |   ",
    "   |  |    (\\      /)  (\\      /)  |  |   ",
    "   |  |     \\`.__.' /   \\`.__.' /   |  |   ",
    "   |  |      `-..-'       `-..-'    |  |   ",
    "   |  |                            |  |   ",
    "   |  |   The Keeper awaits...     |  |   ",
    "   |  |____________________________|  |   ",
    "   |____________________________________|  ",
    "                                           ",
    "        [    COUNTER    ]                  ",
    "       ______________________              ",
    "      |                      |             ",
    "      |   Welcome, traveler  |             ",
    "      |______________________|             ",
};

static void DrawStringAt(int x, int y, const std::string& s) {
    SetCursor(x, y);
    std::cout << s;
}

void DrawShop(Shop* shop, const Player& player) {
    // 1) Clear the map area (left side) only
    for (int y = 0; y < MAP_HEIGHT; ++y) {
        SetCursor(0, y);
        std::cout << std::string(MAP_WIDTH, ' ');
    }

    // compute art position to center in map area
    int artH = static_cast<int>(SHOPKEEPER_ART.size());
    int artW = 0;
    for (auto &line : SHOPKEEPER_ART) if ((int)line.size() > artW) artW = (int)line.size();

    int startY = std::max(0, (MAP_HEIGHT - artH) / 2);
    int startX = std::max(0, (MAP_WIDTH - artW) / 2);

    // draw art
    SetColor(7);
    for (int i = 0; i < artH && (startY + i) < MAP_HEIGHT; ++i) {
        std::string row = SHOPKEEPER_ART[i];
        if ((int)row.size() > MAP_WIDTH - startX) row = row.substr(0, std::max(0, MAP_WIDTH - startX));
        DrawStringAt(startX, startY + i, row);
    }

    // NOTE: we intentionally do NOT clear the menu area here.
    // ShopUI::Render will fully clear and redraw the right-side menu area.
    SetColor(7);
}