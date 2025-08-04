#include "HUDRenderer.h"
#include "ConsoleUtils.h"
#include <iostream>


void DrawHUD(const GameState& state, const Player& player) {
    SetCursor(65, 0);
    std::cout << "WEEK: " << state.currentWeek;
    SetCursor(65, 1);
    std::cout << "DAY: " << state.currentDay;
    SetCursor(65, 2);
    std::cout << "AP: " << state.actionPoints;
    SetCursor(65, 3);
    std::cout << "ZONE: " << player.currentZone;
}