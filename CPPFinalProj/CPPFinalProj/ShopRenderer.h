#pragma once
#include "Shop.h"
#include "Player.h"

// Renders the shop screen: ASCII art in the left map area, shop menu on the right.
// If shop == nullptr this draws a placeholder shop screen.
void DrawShop(Shop* shop, const Player& player);
