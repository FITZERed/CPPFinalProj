#include "InputHandler.h"
#include "Player.h"
#include <conio.h>
#include "MapManager.h"

void HandleInput(Player& player, MapManager& map, GameState& gameState) {
    if (!_kbhit()) return;

    char key = _getch();
    switch (key) {
    case 'w': player.Move(0, -1, map, gameState); break;
    case 's': player.Move(0, 1, map, gameState); break;
    case 'a': player.Move(-1, 0, map, gameState); break;
    case 'd': player.Move(1, 0, map, gameState); break;
    }
}