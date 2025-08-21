#pragma once
#include "Medal.h"
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

class Player;

enum class NPCType {
    TRADER,      // Обычный торговец
    COLLECTOR,   // Коллекционер
    VETERAN,     // Ветеран
    HOBBYIST     // Любитель
};

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
    float bargainDifficulty;
    float gullibility;
    std::vector<Medal> medalsForSale;
    int money; // Деньги NPC
    NPCType type;
    NPC(const std::wstring& name, const std::wstring& backstory, NPCType type,
        float bargainDifficulty, float gullibility, int money = 0);


    std::wstring GetDialogResponse(BargainTactic, bool success);
    float CalculateTacticSuccessChance(BargainTactic tactic, const Player& player) const;

    void AddMedal(const Medal& medal);
    bool TryToCheat(Player& player, const Medal& medal);
    int Bargain(int initialPrice, float playerSkill);
};