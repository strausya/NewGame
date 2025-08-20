#pragma once
#include <string>

class Game;

class SaveSystem {
public:
    static void Save(const Game& game, const std::wstring& filename = L"save.txt");
    static bool Load(Game& game, const std::wstring& filename = L"save.txt");
};