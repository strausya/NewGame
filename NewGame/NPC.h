#pragma once
#include "Medal.h"
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

class Player;

enum class BargainTactic {
    BLUFF,      // Блеф
    FLATTERY,   // Лесть
    THREAT,     // Угроза
    REASON,     // Разумные доводы
    PATIENCE    // Терпение
};

class NPC {
private:
    std::map<BargainTactic, std::vector<std::wstring>> dialogLines;
    void InitializeDialogs();

public:
    std::wstring name;
    std::wstring backstory;
    float bargainDifficulty; // Сложность торга (0-1)
    float gullibility; // Вероятность поверить обману (0-1)
    std::vector<Medal> medalsForSale;
    int money; // Деньги NPC

    std::wstring GetDialogResponse(BargainTactic tactic, bool success);
    float CalculateTacticSuccessChance(BargainTactic tactic, const Player& player);

    NPC(const std::wstring& name, const std::wstring& backstory,
        float bargainDifficulty, float gullibility, int money);

    void AddMedal(const Medal& medal);
    bool TryToCheat(Player& player, const Medal& medal); // Попытка обмануть
    int Bargain(int initialPrice, float playerSkill); // Торг
};