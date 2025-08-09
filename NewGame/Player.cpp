#include "Player.h"
#include "NPC.h"
#include "ConsoleColors.h"
#include "MedalManager.h"
#include "Medal.h"
#include "Location.h"
#include <iostream>
#include <random>

void Player::Trade(Location& currentLocation) {
    float weatherModifier = 1.0f;

        if (this->inventory.IsEmpty()) {
            std::wcout << L"У тебя нет медалей для продажи.\n";
            return;
        }

        this->ShowInventory();

        // Выбор медали
        std::wcout << L"Выбери медаль для продажи (1-" << this->inventory.Size() << "): ";
        size_t choice;
        std::wcin >> choice;
        Medal selectedMedal = this->inventory.GetMedal(choice - 1);


        if (choice < 1 || choice > this->inventory.Size()) {
            std::wcout << L"Неверный выбор!\n";
            return;
        }

        MedalManager manager;

        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 99);

        bool buyerIsExpert = (dis(gen) < currentLocation.fakeDetectionChance * 100);
        int basePrice = manager.GetMarketValue(selectedMedal, this->reputation, buyerIsExpert);
        int finalPrice = static_cast<int>(basePrice * currentLocation.priceModifier);

        // Выбор NPC для торговли (если есть)
        if (!currentLocation.npcs.empty()) {
            NPC& npc = currentLocation.npcs[rand() % currentLocation.npcs.size()];
            std::wcout << L"\n" << npc.name << L" рассматривает твою медаль...\n";
            std::wcout << L"Он предлагает " << basePrice << L" руб. Будешь торговаться? (1-Да, 0-Нет): ";

            int bargainChoice;
            std::wcin >> bargainChoice;

            if (bargainChoice == 1) {
                StartBargainDialogue(npc, selectedMedal, finalPrice);
                return;
            }
        }

        // Дополнительный торг на Стахе
        if (currentLocation.type == LocationType::Stakha && rand() % 100 < 30) {
            finalPrice += static_cast<int>(finalPrice * 0.5f);
            std::wcout << L"Ты удачно выпросил больше денег!\n";
        }

        // Применяем погодный модификатор
        finalPrice = static_cast<int>(finalPrice * weatherModifier);

        if (weatherModifier != 1.0f) {
            std::wcout << L"(Погода повлияла на цену: "
                << (weatherModifier > 1.0f ? L"+" : L"")
                << (weatherModifier - 1.0f) * 100 << L"%)\n";
        }

        // Проверка на фейк
        if (buyerIsExpert && selectedMedal.isFake) {
            std::wcout << L"Покупатель распознал подделку! Медаль конфискована.\n";
            this->inventory.RemoveByIndex(choice - 1);
            this->reputation -= 10;
            return;
        }

        // Продажа
        this->money += finalPrice;
        this->inventory.RemoveByIndex(choice - 1);

        std::wcout << L"Ты продал " << selectedMedal.name << L" за " << finalPrice << L" рублей.\n";
        std::wcout << L"Теперь у тебя " << this->money << L" рублей.\n";

        // Репутация
        if (finalPrice > selectedMedal.minPrice * 2) {
            this->reputation += 5;
            ConsoleColors::SetColor(ConsoleColors::GREEN);
            std::wcout << L"Ты удачно поторговался! Репутация +5 (" << this->reputation << L").\n";
            ConsoleColors::Reset();
        }
        else if (finalPrice < selectedMedal.minPrice * 0.8) {
            this->reputation -= 2;
            ConsoleColors::SetColor(ConsoleColors::RED);
            std::wcout << L"Ты продешевил! Репутация -2 (" << this->reputation << L").\n";
            ConsoleColors::Reset();
        }
}

void Player::StartBargainDialogue(NPC& npc, Medal& medal, int& currentPrice) {
    std::wcout << L"\n=== ТОРГ ===\n";
    std::wcout << L"Текущая цена: " << currentPrice << L" руб.\n\n";

    while (true) {
        std::wcout << L"Выбери тактику:\n";
        std::wcout << L"1. Блеф (шанс: " << npc.CalculateTacticSuccessChance(BargainTactic::BLUFF, *this) * 100 << L"%)\n";
        std::wcout << L"2. Лесть (шанс: " << npc.CalculateTacticSuccessChance(BargainTactic::FLATTERY, *this) * 100 << L"%)\n";
        std::wcout << L"3. Угроза (шанс: " << npc.CalculateTacticSuccessChance(BargainTactic::THREAT, *this) * 100 << L"%)\n";
        std::wcout << L"4. Разумные доводы (шанс: " << npc.CalculateTacticSuccessChance(BargainTactic::REASON, *this) * 100 << L"%)\n";
        std::wcout << L"5. Терпение (шанс: " << npc.CalculateTacticSuccessChance(BargainTactic::PATIENCE, *this) * 100 << L"%)\n";
        std::wcout << L"0. Принять цену\n";
        std::wcout << L"Выбор: ";

        int tacticChoice;
        std::wcin >> tacticChoice;
        BargainTactic tactic = static_cast<BargainTactic>(tacticChoice - 1);


        if (tacticChoice == 0) break;
        if (tacticChoice < 1 || tacticChoice > 5) {
            std::wcout << L"Неверный выбор!\n";
            continue;
        }

        //BargainTactic tactic = static_cast<BargainTactic>(tacticChoice - 1);
        float successChance = npc.CalculateTacticSuccessChance(tactic, *this);

        // Вывод реплики игрока
        std::wcout << L"\nТы: " << npc.GetDialogResponse(tactic, false) << L"\n";

        // Проверка успеха
        bool success = (rand() / static_cast<float>(RAND_MAX)) < successChance;

        // Ответ NPC
        std::wcout << npc.name << L": " << npc.GetDialogResponse(tactic, success) << L"\n";

        if (success) {
            // Успешный торг
            float priceModifier = 1.0f + 0.1f * (1.0f - successChance);
            currentPrice = static_cast<int>(currentPrice * priceModifier);
            std::wcout << L"Новая цена: " << currentPrice << L" руб. (+" << (priceModifier - 1.0f) * 100 << L"%)\n";

            // Обновление репутации
            if (tactic == BargainTactic::THREAT) {
                this->reputation -= 3;
            }
            else if (tactic == BargainTactic::FLATTERY) {
                this->reputation += 1;
            }
        }
        else {
            // Неудачный торг
            this->reputation -= 1;
            std::wcout << L"Цена осталась прежней: " << currentPrice << L" руб.\n";
        }

        std::wcout << L"Твоя репутация теперь: " << this->reputation << L"\n\n";
    }

    // Финальное решение
    std::wcout << L"\nПринять предложение " << currentPrice << L" руб.? (1-Да, 0-Нет): ";
    int finalChoice;
    std::wcin >> finalChoice;

    if (finalChoice == 1) {
        this->money += currentPrice;
        // Inventory removal will be handled in Trade function
        std::wcout << L"Сделка заключена!\n";
    }
    else {
        std::wcout << L"Торг прекращен.\n";
    }
}

void Player::EatFood() {
    if (money >= 50) {
        money -= 50;
        hunger -= 30;
        if (hunger < 0) hunger = 0;
        std::wcout << L"Ты поел чебурек с вокзала.\n";
    }
    else {
        std::wcout << L"Нет денег на еду.\n";
    }
}

void Player::Rest() {
    if (money >= 20) { // Добавляем небольшую стоимость отдыха
        money -= 20;
        fatigue -= 40;
        if (fatigue < 0) fatigue = 0;
        std::wcout << L"Ты снял комнату в хостеле и хорошо отдохнул.\n";
    }
    else {
        fatigue -= 20; // Бесплатный отдых менее эффективен
        if (fatigue < 0) fatigue = 0;
        std::wcout << L"Ты поспал на лавке. Сон тревожный, но помог.\n";
    }

    if (fatigue < 0) fatigue = 0;

    // Ограничение максимума усталости
    if (fatigue > 100) fatigue = 100;
}

void Player::AddMedal(const Medal& medal) {
    inventory.Add(medal);
}

void Player::ShowInventory() const {
    if (inventory.IsEmpty()) {
        std::wcout << L"Инвентарь пуст!\n";
        return;
    }

    std::wcout << L"=== Твой инвентарь ===\n";
    for (size_t i = 0; i < inventory.Size(); ++i) {
        Medal m = inventory.GetMedal(i);
        std::wcout << i + 1 << L". " << m.name
            << L" (" << m.condition << L")"
            << (m.isFake ? L" [Фейк]" : L"") << L"\n";
    }
}

void Player::ShowStats() const {
    ConsoleColors::SetColor(ConsoleColors::YELLOW);
    std::wcout << L"┌───────────────────────────────────────┐\n";
    std::wcout << L"│ ";
    ConsoleColors::SetColor(ConsoleColors::WHITE);
    std::wcout << L"Состояние игрока:";
    ConsoleColors::SetColor(ConsoleColors::YELLOW);
    std::wcout << L"                                        │\n";
    std::wcout << L"│                                       │\n";

    // Деньги
    std::wcout << L"│ Деньги: ";
    ConsoleColors::SetColor(ConsoleColors::GREEN);
    std::wcout << money << L" руб.";
    ConsoleColors::SetColor(ConsoleColors::YELLOW);
    std::wcout << L"                                        │\n";

    // Голод
    std::wcout << L"│ Голод: ";
    if (hunger > 70) ConsoleColors::SetColor(ConsoleColors::RED);
    else ConsoleColors::SetColor(ConsoleColors::WHITE);
    std::wcout << hunger << L"%";
    ConsoleColors::SetColor(ConsoleColors::YELLOW);
    std::wcout << L"                                        │\n";

    // Усталость
    std::wcout << L"│ Усталость: ";
    if (fatigue > 70 || fatigue == 100) ConsoleColors::SetColor(ConsoleColors::RED);
    else ConsoleColors::SetColor(ConsoleColors::WHITE);
    std::wcout << fatigue << L"%";
    ConsoleColors::SetColor(ConsoleColors::YELLOW);
    std::wcout << L"                                        │\n";

    // Репутация
    std::wcout << L"│ Репутация: ";
    if (reputation > 50) ConsoleColors::SetColor(ConsoleColors::GREEN);
    else if (reputation < 0) ConsoleColors::SetColor(ConsoleColors::RED);
    else ConsoleColors::SetColor(ConsoleColors::WHITE);
    std::wcout << reputation;
    ConsoleColors::SetColor(ConsoleColors::YELLOW);
    std::wcout << L"                                        │\n";

    std::wcout << L"└───────────────────────────────────────┘\n";
    ConsoleColors::Reset();
}
