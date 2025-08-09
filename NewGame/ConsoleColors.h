#pragma once

class ConsoleColors {
public:
    static const unsigned short RED;
    static const unsigned short GREEN;
    static const unsigned short BLUE;
    static const unsigned short YELLOW;
    static const unsigned short WHITE;
    static const unsigned short CYAN;
    static const unsigned short MAGENTA;

    static void SetColor(unsigned short color);
    static void Reset();
};