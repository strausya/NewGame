#pragma once
#include <string>

class TextPrinter {
public:
    static void Print(const std::wstring& text, int delayMs = 15);
    static void PrintLine(const std::wstring& text, int delayMs = 15);
};