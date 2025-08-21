#include "Player.h"
#include "DialogueSystem.h"
#include "NPC.h"
#include "ConsoleColors.h"
#include "MedalManager.h"
#include "Medal.h"
#include "Location.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <random>

static int ReadIntSafe() {
    int v = 0;
    if (!(std::wcin >> v)) {
        std::wcin.clear();
        std::wstring tmp;
        std::getline(std::wcin, tmp);
        return 0;
    }
    return v;
}

void Player::Trade(Location& currentLocation) {
    if (inventory.IsEmpty()) {
        std::wcout << L"У тебя нет медалей для продажи.\n";
        return;
    }

    std::wcout << L"Вы решили торговать с NPC.\n";

    // Выбор NPC
    if (currentLocation.npcs.empty()) {
        std::wcout << L"В этой локации нет никого для торговли.\n";
        return;
    }

    std::wcout << L"Выберите NPC для торговли:\n";
    for (size_t i = 0; i < currentLocation.npcs.size(); ++i) {
        std::wcout << i + 1 << L") " << currentLocation.npcs[i].name << L"\n";
    }

    int npcChoice = ReadIntSafe();
    if (npcChoice < 1 || npcChoice > static_cast<int>(currentLocation.npcs.size())) {
        std::wcout << L"Неверный выбор.\n";
        return;
    }

    NPC& npc = currentLocation.npcs[npcChoice - 1];

    // Выбор медали
    std::wcout << L"Ваши медали:\n";
    for (size_t i = 0; i < inventory.Size(); ++i) {
        const Medal& m = inventory.GetMedal(i);
        std::wcout << i + 1 << L") " << m.name << L" (" << m.condition << L") "
            << L"Цена: " << m.maxPrice << L"₽\n";
    }

    int medalChoice = 0;
    std::wcout << L"Выберите медаль для продажи (1-" << inventory.Size() << L"): ";
    medalChoice = ReadIntSafe();
    if (medalChoice < 1 || medalChoice > static_cast<int>(inventory.Size())) {
        std::wcout << L"Неверный выбор медали.\n";
        return;
    }

    size_t medalIndex = medalChoice - 1;
    Medal selectedMedal = inventory.GetMedal(medalIndex);

    // Начальная цена для диалога
    int startingPrice = selectedMedal.maxPrice;

    // Запуск многоходового диалога
    DialogueSystem::StartMultiRoundDialogue(*this, npc, medalIndex, startingPrice);

    // После диалога показываем финальные результаты
    std::wcout << L"\nВаш текущий баланс: " << money << L"₽\n";
    std::wcout << L"Репутация: " << reputation << L"\n";
}

void Player::BuyFromNPC(Location& currentLocation) {
    if (currentLocation.npcs.empty()) {
        std::wcout << L"Здесь нет продавцов.\n";
        return;
    }

    std::wcout << L"Вы решили торговать с NPC.\n";

    // Выбор NPC
    std::wcout << L"Выберите NPC для покупки:\n";
    for (size_t i = 0; i < currentLocation.npcs.size(); ++i) {
        std::wcout << i + 1 << L". " << currentLocation.npcs[i].name << L"\n";
    }

    int npcChoice = ReadIntSafe();
    if (npcChoice < 1 || npcChoice > static_cast<int>(currentLocation.npcs.size())) {
        std::wcout << L"Неверный выбор.\n";
        return;
    }

    NPC& npc = currentLocation.npcs[npcChoice - 1];

    if (npc.medalsForSale.empty()) {
        std::wcout << L"У этого NPC нет медалей на продажу.\n";
        return;
    }

    // Показать список медалей
    std::wcout << L"Медали у " << npc.name << L":\n";
    for (size_t i = 0; i < npc.medalsForSale.size(); ++i) {
        std::wcout << i + 1 << L". " << npc.medalsForSale[i].name
            << L" (" << npc.medalsForSale[i].condition << L") "
            << npc.medalsForSale[i].minPrice << L" - "
            << npc.medalsForSale[i].maxPrice << L" руб.\n";
    }

    int medalChoice = ReadIntSafe();
    if (medalChoice < 1 || medalChoice > static_cast<int>(npc.medalsForSale.size())) {
        std::wcout << L"Неверный выбор.\n";
        return;
    }

    size_t medalIndex = medalChoice - 1;
    Medal& selectedMedal = npc.medalsForSale[medalIndex];
    int startingPrice = selectedMedal.maxPrice;

    // Запуск многоходового диалога
    DialogueSystem::StartMultiRoundDialogue(*this, npc, medalIndex, startingPrice);

    // После диалога проверяем деньги и покупку
    MedalManager manager;
    int price = manager.GetMarketValue(selectedMedal, reputation, false);

    if (money >= price) {
        money -= price;
        inventory.Add(selectedMedal);
        npc.medalsForSale.erase(npc.medalsForSale.begin() + medalIndex);
        std::wcout << L"Вы купили медаль!\n";
        std::wcout << L"Баланс: " << money << L"₽\n";
    }
    else {
        std::wcout << L"У вас недостаточно денег.\n";
    }
}

void Player::StartBargainDialogue(NPC& npc, Medal& medal, int& currentPrice) {
    std::wcout << L"\n=== ТОРГ ===\n";
    std::wcout << L"Текущая цена: " << currentPrice << L" руб.\n\n";

    static unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    static std::mt19937 generator(seed);

    while (true) {
        std::wcout << L"Выбери тактику:\n";
        std::wcout << L"1. Блеф\n";
        std::wcout << L"2. Лесть\n";
        std::wcout << L"3. Угроза\n";
        std::wcout << L"4. Разумные доводы\n";
        std::wcout << L"5. Терпение\n";
        std::wcout << L"0. Принять цену\n";
        std::wcout << L"Выбор: ";

        int tacticChoice = ReadIntSafe();

        if (tacticChoice == 0) break;
        if (tacticChoice < 1 || tacticChoice > 5) {
            std::wcout << L"Неверный выбор!\n";
            continue;
        }

        BargainTactic tactic = static_cast<BargainTactic>(tacticChoice - 1);
        float successChance = npc.CalculateTacticSuccessChance(tactic, *this);

        // Вывод реплики игрока
        std::wcout << L"\nТы: " << npc.GetDialogResponse(tactic, false) << L"\n";

        // Простая проверка успеха
        int randomValue = generator() % 100;
        bool success = (randomValue < (successChance * 100));

        // Ответ NPC
        std::wcout << npc.name << L": " << npc.GetDialogResponse(tactic, success) << L"\n";

        if (success) {
            // Успешный торг
            currentPrice = static_cast<int>(currentPrice * 0.9f); // 10% скидка
            std::wcout << L"Новая цена: " << currentPrice << L" руб. (-10%)\n";
            reputation += 1;
        }
        else {
            reputation -= 1;
            std::wcout << L"Цена осталась прежней: " << currentPrice << L" руб.\n";
        }

        std::wcout << L"Твоя репутация теперь: " << reputation << L"\n\n";
    }

    // Финальное решение
    std::wcout << L"\nПринять предложение " << currentPrice << L" руб.? (1-Да, 0-Нет): ";
    int finalChoice = ReadIntSafe();

    if (finalChoice == 1) {
        money += currentPrice;
        std::wcout << L"Сделка заключена!\n";
    }
    else {
        std::wcout << L"Торг прекращен.\n";
    }
}

void Player::EatFood() {
    if (money >= 100) {
        money -= 50;
        hunger -= 50;
        if (hunger < 0) hunger = 0;
        std::wcout << L"Ты поел чебурек с вокзала.\n";
    }
    else {
        std::wcout << L"Нет денег на еду.\n";
    }
}

void Player::Rest() {
    if (money >= 20) { 
        money -= 20;
        fatigue -= 40;
        if (fatigue < 0) fatigue = 0;
        std::wcout << L"Ты снял комнату в хостеле и хорошо отдохнул.\n";
    }
    else {
        fatigue -= 20;
        if (fatigue < 0) fatigue = 0;
        std::wcout << L"Ты поспал на лавке. Сон тревожный, но помог.\n";
    }

    if (fatigue < 0) fatigue = 0;

    if (fatigue > 100) fatigue = 100;
}

void Player::AddMedal(const Medal& medal) {
    inventory.Add(medal);
    ApplyMedalEffects(medal);
}

void Player::ShowInventory() const {
    if (inventory.IsEmpty()) {
        std::wcout << L"Инвентарь пуст!\n";
        return;
    }

    std::wcout << L"=== Твой инвентарь ===\n";
    for (size_t i = 0; i < inventory.Size(); ++i) {
        const Medal m = inventory.GetMedal(i);
        std::wcout << i + 1 << L". " << m.name
            << L" (" << m.condition << L")"
            << (m.isFake ? L" [Фейк]" : L"") << L"\n";
    }
}

/*void Player::InitPrevStats() {
    prevMoney = money;
    prevHunger = hunger;
    prevFatigue = fatigue;
    prevReputation = reputation;
}*/

/*void Player::ShowChangedStats() {
    if (money != prevMoney) {
        std::wcout << L"Деньги: " << money << L" руб.\n";
        prevMoney = money;
    }
    if (hunger != prevHunger) {
        std::wcout << L"Голод: " << hunger << L"%\n";
        prevHunger = hunger;
    }
    if (fatigue != prevFatigue) {
        std::wcout << L"Усталость: " << fatigue << L"%\n";
        prevFatigue = fatigue;
    }
    if (reputation != prevReputation) {
        std::wcout << L"Репутация: " << reputation << L"\n";
        prevReputation = reputation;
    }
}*/

void Player::ShowStats() const {
    ConsoleColors::SetColor(ConsoleColors::YELLOW);
    std::wcout << L"┌───────────────────────────────────────┐\n";
    std::wcout << L"│ ";
    ConsoleColors::SetColor(ConsoleColors::WHITE);
    std::wcout << L"Состояние игрока";
    ConsoleColors::SetColor(ConsoleColors::YELLOW);
    std::wcout << L"                      │\n";
    std::wcout << L"├───────────────────────────────────────┤\n";

    // Деньги
    std::wcout << L"│ Деньги:      ";
    ConsoleColors::SetColor(ConsoleColors::GREEN);
    std::wcout << std::setw(8) << money;
    ConsoleColors::SetColor(ConsoleColors::YELLOW);
    std::wcout << L" руб.            │\n";

    // Голод
    std::wcout << L"│ Голод:       ";
    if (hunger > 70) ConsoleColors::SetColor(ConsoleColors::RED);
    else ConsoleColors::SetColor(ConsoleColors::WHITE);
    std::wcout << std::setw(8) << hunger << L"%";
    ConsoleColors::SetColor(ConsoleColors::YELLOW);
    std::wcout << L"                │\n";

    // Усталость
    std::wcout << L"│ Усталость:   ";
    if (fatigue > 70) ConsoleColors::SetColor(ConsoleColors::RED);
    else ConsoleColors::SetColor(ConsoleColors::WHITE);
    std::wcout << std::setw(8) << fatigue << L"%";
    ConsoleColors::SetColor(ConsoleColors::YELLOW);
    std::wcout << L"                │\n";

    // Репутация
    std::wcout << L"│ Репутация:   ";
    if (reputation > 50) ConsoleColors::SetColor(ConsoleColors::GREEN);
    else if (reputation < 0) ConsoleColors::SetColor(ConsoleColors::RED);
    else ConsoleColors::SetColor(ConsoleColors::WHITE);
    std::wcout << std::setw(8) << reputation;
    ConsoleColors::SetColor(ConsoleColors::YELLOW);
    std::wcout << L"                 │\n";

    std::wcout << L"└───────────────────────────────────────┘\n";
    ConsoleColors::Reset();
}

void Player::ApplyMedalEffects(const Medal& medal) {
    switch (medal.effect) {
    case MedalEffect::ReputationBoost:
        reputation += medal.effectValue;
        std::wcout << L"Репутация выросла на " << medal.effectValue << L"\n";
        break;
    case MedalEffect::HungerReduction:
        hunger = std::max(0, hunger - medal.effectValue);
        std::wcout << L"Сытость улучшена на " << medal.effectValue << L"\n";
        break;
    case MedalEffect::FatigueRecovery:
        fatigue = std::max(0, fatigue - medal.effectValue);
        std::wcout << L"Усталость уменьшена на " << medal.effectValue << L"\n";
        break;
    case MedalEffect::BargainBonus:
        reputation += medal.effectValue / 2;
        std::wcout << L"Торговые навыки улучшены!\n";
        break;
    case MedalEffect::MoneyBonus:
        money += medal.effectValue;
        std::wcout << L"Получен бонус: +" << medal.effectValue << L"₽\n";
        break;
    default:
        break;
    }
}