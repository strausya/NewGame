#pragma once
#include "Medal.h"
#include "Inventory.h"
#include "Location.h"
#include "Weather.h"
#include <vector>
#include <string>
#include <iostream>

class NPC;

class Player {
private:
    void StartBargainDialogue(NPC& npc, Medal& medal, int& currentPrice);


public:
    int money = 0;
    int hunger = 0;
    int fatigue = 0;
    int reputation = 0;
    Inventory inventory;

    void AddMedal(const Medal& medal);
    void ShowInventory() const;
    void Trade(Location& currentLocation);
    void EatFood();
    void Rest();
    void ShowStats() const;
};
