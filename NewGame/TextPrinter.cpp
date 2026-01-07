#include "TextPrinter.h"
#include <iostream>
#include <thread>
#include <chrono>

void TextPrinter::Print(const std::wstring& text, int delayMs) {
    for (wchar_t c : text) {
        std::wcout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
    }
}

void TextPrinter::PrintLine(const std::wstring& text, int delayMs) {
    Print(text, delayMs);
    std::wcout << L'\n';
}
