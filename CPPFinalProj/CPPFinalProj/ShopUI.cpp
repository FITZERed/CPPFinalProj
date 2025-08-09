#include "ShopUI.h"
#include "ConsoleUtils.h"
#include "config.h"
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

namespace ShopUI {

// Internal state (unchanged)
enum class UIMode { MainMenu, BuyMenu, SellMenu };
static UIMode uiMode = UIMode::MainMenu;
static int mainIndex = 0; // 0=Buy,1=Sell,2=Leave
static int buyAmount = 1;
static int sellIndex = 0;
static int sellAmount = 1;
static std::vector<int> sellResources;
static std::string statusMessage = "";

// Menu layout constants
static const int MENU_X = MAP_WIDTH + 2;
static const int MENU_WIDTH = 40; // must match clearing width

// Pad a line to MENU_WIDTH (ensures leftover chars are overwritten)
static std::string PadLine(const std::string &s) {
    if ((int)s.size() >= MENU_WIDTH) return s.substr(0, MENU_WIDTH);
    return s + std::string(MENU_WIDTH - (int)s.size(), ' ');
}

// Print a padded menu line at (MENU_X, row)
static void MenuPrint(int row, const std::string &text) {
    SetCursor(MENU_X, row);
    std::cout << PadLine(text);
}

// Price helpers
static int UnitPriceSellToPlayer(ResourceType r) {
    return BASE_PRICES[static_cast<int>(r)];
}
static int UnitPriceBuyFromPlayer(ResourceType r, int shopPriority) {
    float mul = PRIORITY_MULTIPLIERS[shopPriority];
    double price = static_cast<double>(BASE_PRICES[static_cast<int>(r)]) * mul;
    int iprice = std::max(1, (int)std::round(price));
    return iprice;
}

static void BuildSellResourceList(Shop* shop) {
    sellResources.clear();
    for (int r = 0; r < static_cast<int>(ResourceType::Count); ++r) {
        if (static_cast<ResourceType>(r) == shop->mainResource) continue;
        sellResources.push_back(r);
    }
    sellIndex = 0;
    sellAmount = 1;
}

void EnterShop(Shop* shop, Player* player, GameState* state) {
    uiMode = UIMode::MainMenu;
    mainIndex = 0;
    buyAmount = 1;
    sellIndex = 0;
    sellAmount = 1;
    statusMessage.clear();
    sellResources.clear();
    if (shop) BuildSellResourceList(shop);
}

// Clear the entire menu area (MENU_X..MENU_X+MENU_WIDTH-1, rows 0..MAP_HEIGHT-1)
static void ClearMenuArea() {
    std::string blank = std::string(MENU_WIDTH, ' ');
    for (int y = 0; y < MAP_HEIGHT; ++y) {
        SetCursor(MENU_X, y);
        std::cout << blank;
    }
}

// Render menu once (called every frame from main loop)
void Render(Shop* shop, const Player& player) {
    // 1) Clear the right menu area completely
    ClearMenuArea();

    int row = 0;
    MenuPrint(row++, "================ SHOP ================");
    if (shop) MenuPrint(row++, "Name: " + shop->name);
    else MenuPrint(row++, "Name: (none)");
    MenuPrint(row++, "");
    MenuPrint(row++, "Player Money: " + std::to_string(player.inventory.money));
    MenuPrint(row++, "");

    if (uiMode == UIMode::MainMenu) {
        const char* opts[3] = { "Buy", "Sell", "Leave" };
        for (int i = 0; i < 3; ++i) {
            std::string line = (mainIndex == i ? ">> " : "   ");
            line += opts[i];
            MenuPrint(row++, line);
        }
        row++;
        MenuPrint(row++, "Use W/S to move, Enter to select, Q to leave");
    }
    else if (uiMode == UIMode::BuyMenu) {
        ResourceType r = shop->mainResource;
        int unit = UnitPriceSellToPlayer(r);
        int shopStock = shop->inventory.GetAmount(r);
        MenuPrint(row++, "Buy " + std::string(RESOURCE_NAMES[static_cast<int>(r)]));
        MenuPrint(row++, "Unit: " + std::to_string(unit) + "  ShopStock: " + std::to_string(shopStock));
        MenuPrint(row++, "");
        MenuPrint(row++, "Amount: " + std::to_string(buyAmount) + "   (A/D to change)");
        MenuPrint(row++, "Total: " + std::to_string(unit * buyAmount));
        row++;
        MenuPrint(row++, "Enter to purchase, Q=Back");
    }
    else if (uiMode == UIMode::SellMenu) {
        MenuPrint(row++, "Sell (choose resource):");
        for (int i = 0; i < (int)sellResources.size(); ++i) {
            int r = sellResources[i];
            bool selected = (sellIndex == i);
            std::string prefix = selected ? ">> " : "   ";
            int priority = shop->priorities[r];
            int unit = UnitPriceBuyFromPlayer(static_cast<ResourceType>(r), priority);
            int playerHave = player.inventory.GetAmount(static_cast<ResourceType>(r));
            int shopMoney = shop->inventory.money;
            int maxSell = 0;
            if (unit > 0) maxSell = std::min(playerHave, shopMoney / unit);
            std::string line = prefix + std::string(RESOURCE_NAMES[r]) + "  Unit: " + std::to_string(unit)
                + "  You: " + std::to_string(playerHave) + "  MaxSell: " + std::to_string(maxSell)
                + "  CurrAmount: " + std::to_string(sellAmount);
            MenuPrint(row++, line);
        }
        row++;
        MenuPrint(row++, "W/S up/down  A/D change amount  Enter sell  Q=Back");
    }

    row += 1;
    MenuPrint(row++, "---------------------------------------");
    MenuPrint(row++, "Status: " + statusMessage);
    MenuPrint(row++, "");

    // flush optional (console usually flushes on next write)
    std::cout << std::flush;
}

// Transaction helpers
static void TryBuy(Shop* shop, Player& player, GameState& state) {
    ResourceType r = shop->mainResource;
    int unit = UnitPriceSellToPlayer(r);
    int total = unit * buyAmount;
    int shopStock = shop->inventory.GetAmount(r);

    if (state.actionPoints <= 0) {
        statusMessage = "Not enough AP for transaction.";
        return;
    }
    if (buyAmount <= 0) {
        statusMessage = "Amount must be >= 1.";
        return;
    }
    if (shopStock < buyAmount) {
        statusMessage = "Shop doesn't have enough stock.";
        return;
    }
    if (player.inventory.money < total) {
        statusMessage = "You don't have enough money.";
        return;
    }

    player.inventory.money -= total;
    shop->inventory.money += total;
    shop->inventory.Remove(r, buyAmount);  // shop sells to player
    player.inventory.Add(r, buyAmount);

    state.actionPoints -= 1;
    statusMessage = "Bought " + std::to_string(buyAmount) + " " + std::string(RESOURCE_NAMES[static_cast<int>(r)]) + ".";
}

static void TrySell(Shop* shop, Player& player, GameState& state) {
    if (sellIndex < 0 || sellIndex >= (int)sellResources.size()) {
        statusMessage = "No resource selected.";
        return;
    }
    int r = sellResources[sellIndex];
    int priority = shop->priorities[r];
    int unit = UnitPriceBuyFromPlayer(static_cast<ResourceType>(r), priority);
    int total = unit * sellAmount;

    if (state.actionPoints <= 0) {
        statusMessage = "Not enough AP for transaction.";
        return;
    }
    if (sellAmount <= 0) {
        statusMessage = "Amount must be >= 1.";
        return;
    }
    int playerHave = player.inventory.GetAmount(static_cast<ResourceType>(r));
    if (playerHave < sellAmount) {
        statusMessage = "You don't have enough of that resource.";
        return;
    }
    if (shop->inventory.money < total) {
        statusMessage = "Shop can't afford that many right now.";
        return;
    }

    player.inventory.Remove(static_cast<ResourceType>(r), sellAmount);
    player.inventory.money += total;
    shop->inventory.money -= total;
    shop->inventory.Add(static_cast<ResourceType>(r), sellAmount);

    state.actionPoints -= 1;
    statusMessage = "Sold " + std::to_string(sellAmount) + " " + std::string(RESOURCE_NAMES[r]) + ".";
}

// HandleInput: update UI state, but DO NOT call Render here
void HandleInput(char key, Shop* shop, Player& player, GameState& state) {
    if (key >= 'A' && key <= 'Z') key = key - 'A' + 'a';

    if (uiMode == UIMode::MainMenu) {
        if (key == 'w') {
            mainIndex = (mainIndex + 3 - 1) % 3;
        } else if (key == 's') {
            mainIndex = (mainIndex + 1) % 3;
        } else if (key == '\r' || key == '\n') {
            if (mainIndex == 0) {
                uiMode = UIMode::BuyMenu;
                buyAmount = 1;
                statusMessage.clear();
            } else if (mainIndex == 1) {
                uiMode = UIMode::SellMenu;
                BuildSellResourceList(shop);
                sellIndex = 0;
                sellAmount = 1;
                statusMessage.clear();
            } else {
                state.mode = GameMode::Market;
                state.currentShop = nullptr;
                statusMessage = "Left shop.";
            }
        } else if (key == 'q') {
            state.mode = GameMode::Market;
            state.currentShop = nullptr;
            statusMessage = "Left shop.";
        }
    }
    else if (uiMode == UIMode::BuyMenu) {
        ResourceType r = shop->mainResource;
        int shopStock = shop->inventory.GetAmount(r);
        int unit = UnitPriceSellToPlayer(r);
        int maxAmount = shopStock;
        if (key == 'a') {
            buyAmount = std::max(1, buyAmount - 1);
        } else if (key == 'd') {
            int moneyMax = player.inventory.money / (unit == 0 ? 1 : unit);
            int effectiveMax = std::max(0, std::min(maxAmount, moneyMax));
            if (effectiveMax < 1) effectiveMax = 1;
            buyAmount = std::min(effectiveMax, buyAmount + 1);
        } else if (key == '\r' || key == '\n') {
            TryBuy(shop, player, state);
        } else if (key == 'q') {
            uiMode = UIMode::MainMenu;
            statusMessage.clear();
        }
    }
    else if (uiMode == UIMode::SellMenu) {
        if (sellResources.empty()) {
            statusMessage = "Shop will not buy any resource.";
            return;
        }
        int rIndex = sellResources[sellIndex];
        int priority = shop->priorities[rIndex];
        int unit = UnitPriceBuyFromPlayer(static_cast<ResourceType>(rIndex), priority);

        if (key == 'w') {
            sellIndex = std::max(0, sellIndex - 1);
            sellAmount = 1;
        } else if (key == 's') {
            sellIndex = std::min((int)sellResources.size() - 1, sellIndex + 1);
            sellAmount = 1;
        } else if (key == 'a') {
            sellAmount = std::max(1, sellAmount - 1);
        } else if (key == 'd') {
            int playerHave = player.inventory.GetAmount(static_cast<ResourceType>(rIndex));
            int shopMoneyMax = (unit == 0) ? playerHave : (shop->inventory.money / unit);
            int effectiveMax = std::max(0, std::min(playerHave, shopMoneyMax));
            if (effectiveMax < 1) effectiveMax = 1;
            sellAmount = std::min(effectiveMax, sellAmount + 1);
        } else if (key == '\r' || key == '\n') {
            TrySell(shop, player, state);
        } else if (key == 'q') {
            uiMode = UIMode::MainMenu;
            statusMessage.clear();
        }
    }

    // NOTE: Do NOT call Render(...) here. Main loop will render once per frame.
}

} // namespace ShopUI