#include "Player.h"
#include "MapManager.h"
#include "GameState.h"
#include "config.h"

void Player::Move(int dx, int dy, MapManager& mapManager, GameState& gameState) {
    int newX = position.X + dx;
    int newY = position.Y + dy;

    if (newX < 0 || newX >= MAP_WIDTH || newY < 0 || newY >= MAP_HEIGHT)
        return;

    char targetTile = mapManager.map[newY][newX].character;

    if (!mapManager.IsWalkable(targetTile))
        return;

    int newZone = mapManager.GetZoneAt(newX, newY);
    if (newZone != currentZone && gameState.actionPoints > 0) {
        gameState.actionPoints--;
        currentZone = newZone;
    }
    else if (newZone != currentZone && gameState.actionPoints <= 0) {
        return; // No AP left to cross zones
    }

    position = { (SHORT)newX, (SHORT)newY };
}