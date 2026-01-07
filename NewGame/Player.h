#pragma once
#include "Medal.h"
#include "Inventory.h"
#include "Location.h"
#include "Weather.h"
#include <vector>
#include <map>
#include <string>
#include <iostream>

class NPC;

class Player {
private:
    int prevMoney = -1;
    int prevHunger = -1;
    int prevFatigue = -1;
    int prevReputation = -1;

public:
    int money = 0;
    int hunger = 0;
    int fatigue = 0;
    int reputation = 0;
    Inventory inventory;
    std::map<std::wstring, int> npcTrust;
    bool StartBargainDialogue(NPC& npc, Medal& medal, int& currentPrice, bool isBuying);

    void AddMedal(const Medal& medal);
    void ShowInventory() const;
    void Trade(Location& currentLocation);
    void EatFood();
    void Rest();
    void ShowStats() const;
    void ShowChangedStats();
    void InitPrevStats();
    void BuyFromNPC(Location& currentLocation);
    int GetTrust(const std::wstring& npcName) const;
    void ChangeTrust(const std::wstring& npcName, int delta);
};
