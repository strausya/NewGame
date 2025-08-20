#include "SaveSystem.h"
#include "Game.h"
#include "Player.h"
#include "Inventory.h"
#include "Medal.h"
#include <fstream>
#include <codecvt>
#include <locale>
#include <sstream>

void SaveSystem::Save(const Game& game, const std::wstring& filename) {
    std::wofstream file(filename);
    file.imbue(std::locale(file.getloc(), new std::codecvt_utf8<wchar_t>));

    if (!file.is_open()) return;

    const Player& player = game.GetPlayer();

    // Сохраняем базовые статы игрока
    file << player.money << L" "
        << player.hunger << L" "
        << player.fatigue << L" "
        << player.reputation << L"\n";

    // Сохраняем медали
    const auto& medals = player.inventory.GetItems();
    file << medals.size() << L"\n";
    for (const auto& medal : medals) {
        file << medal.name << L"|"
            << medal.condition << L"|"
            << (int)medal.tier << L"|"
            << medal.isFake << L"|"
            << medal.effectOnPlayer << L"|"
            << medal.baseCost << L"|"
            << (int)medal.effect << L"|"
            << medal.effectValue
            << L"\n";
    }
}

bool SaveSystem::Load(Game& game, const std::wstring& filename) {
    std::wifstream file(filename);
    file.imbue(std::locale(file.getloc(), new std::codecvt_utf8<wchar_t>));

    if (!file.is_open()) return false;

    Player& player = game.GetPlayer();
    player.inventory = Inventory(); // очистка

    // Загружаем статы игрока
    file >> player.money >> player.hunger >> player.fatigue >> player.reputation;

    // Загружаем медали
    size_t medalCount;
    file >> medalCount;
    file.ignore(); // пропуск \n

    for (size_t i = 0; i < medalCount; i++) {
        std::wstring line;
        std::getline(file, line); // wide-версия getline

        if (line.empty()) continue;

        std::wistringstream iss(line);
        std::wstring token;
        std::vector<std::wstring> parts;

        while (std::getline(iss, token, L'|')) { // тоже wide
            parts.push_back(token);
        }

        if (parts.size() >= 8) {
            Medal medal(
                parts[0],                           // name
                parts[1],                           // condition
                0, 0,                               // min/max (можно 0, если не сохраняем)
                (MedalTier)std::stoi(parts[2]),     // tier
                (parts[3] == L"1"),                 // isFake
                parts[4],                           // effectOnPlayer
                std::stoi(parts[5]),                // baseCost
                (MedalEffect)std::stoi(parts[6]),   // effect
                std::stoi(parts[7])                 // effectValue
            );
            player.inventory.Add(medal);
        }
    }

    return true;
}
