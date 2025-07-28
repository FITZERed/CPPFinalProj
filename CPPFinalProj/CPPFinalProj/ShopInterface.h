#pragma once
enum class ShopMenuState { Main, Buy, Sell };

class ShopInterface {
private:
    Shop* currentShop;
    Player* player;

    ShopMenuState menuState;
    int selectedIndex;
    int selectedAmount;

public:
    void EnterShop(Shop*, Player*);
    void HandleInput(char key);
    void DrawInterface();
};
