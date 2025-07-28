#pragma once
#include "config.h"

enum class GameMode {
    Market,
    Shop,
    Transition
};

struct GameState {
    int currentDay = 1;
    int currentWeek = 1;
    int totalMoneyEarned = 0;
    int playerMoney = STARTING_MONEY;
    int actionPoints = STARTING_ACTION_POINTS;
    GameMode mode = GameMode::Market;
};