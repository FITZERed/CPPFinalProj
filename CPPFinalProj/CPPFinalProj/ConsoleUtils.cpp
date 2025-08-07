#include "ConsoleUtils.h"
#include <iostream> // Needed for std::cout
#include <windows.h>

void SetCursor(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void SetColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void ClearLine(int y) {
    SetCursor(0, y);
    std::cout << std::string(120, ' '); // Clear 120 characters wide
    SetCursor(0, y); // Reset cursor to line start
}