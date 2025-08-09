#include "ConsoleColors.h"
#include <windows.h>

const unsigned short ConsoleColors::RED = FOREGROUND_RED | FOREGROUND_INTENSITY;
const unsigned short ConsoleColors::GREEN = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
const unsigned short ConsoleColors::BLUE = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
const unsigned short ConsoleColors::YELLOW = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
const unsigned short ConsoleColors::WHITE = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
const unsigned short ConsoleColors::CYAN = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
const unsigned short ConsoleColors::MAGENTA = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;

void ConsoleColors::SetColor(unsigned short color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void ConsoleColors::Reset() {
    SetColor(WHITE);
}
