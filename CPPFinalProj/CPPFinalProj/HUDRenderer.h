#pragma once
#include <string>
#include "GameState.h"
#include "Player.h"

class HUDRenderer {
public:
    void DrawHUD(const GameState&, const Player&);
    void LogEvent(const std::string&);
};