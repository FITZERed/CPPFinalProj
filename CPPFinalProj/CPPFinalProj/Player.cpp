#include "Player.h"
#include "MapManager.h"
#include "GameState.h"
#include "config.h"

Player::Player() {
    position = { 0, 0 };
    currentZone = 0; // Default zone, will be set during map load
}

void Player::Move(int dx, int dy, MapManager& map, GameState& gameState) {
    int newX = position.X + dx;
    int newY = position.Y + dy;

    // Bounds check
    if (newX < 0 || newX >= MAP_WIDTH || newY < 0 || newY >= MAP_HEIGHT) {
        return;
    }

    char target = map.GetCharAt(newX, newY);
    if (!map.IsWalkable(target)) {
        return;
    }

    int newZone = map.GetZoneAt(newX, newY);
    
    if (newZone != currentZone) {
        if (gameState.actionPoints <= 0) {
            std::cerr << "Not enough AP to cross zones!\n";
            return;
        }
        gameState.actionPoints--;       //  Spend AP
        currentZone = newZone;          //  Update zone
    }

    position = { (SHORT)newX, (SHORT)newY };  //  Apply movement
}