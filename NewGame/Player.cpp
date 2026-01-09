#include "Player.h"
#include "NPC.h"
#include "ConsoleColors.h"
#include "MedalManager.h"
#include "Medal.h"
#include "Location.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <random>
#include <limits>
#include <cmath>
#include <Windows.h>

static bool WContains(const std::wstring& s, const std::wstring& sub) {
    return s.find(sub) != std::wstring::npos;
}

static std::wstring ShortEffect(const std::wstring& eff) {
    if (eff.empty()) return L"";
    if (eff.size() <= 60) return eff;
    return eff.substr(0, 60) + L"...";
}

// Пассивные бонусы от медалей (минимально, но реально влияет на геймплей)
static void GetMedalPassives(const Player& p, float& bargainBonus, float& fatigueMult, int& repBonus, int& eventBonus) {
    bargainBonus = 0.0f;
    fatigueMult = 1.0f;
    repBonus = 0;
    eventBonus = 0;

    for (const auto& m : p.inventory.GetItems()) {
        if (WContains(m.effectOnPlayer, L"+10% к торгу")) bargainBonus += 0.10f;
        if (WContains(m.effectOnPlayer, L"усталость растёт медленнее")) fatigueMult *= 0.85f;
        if (WContains(m.effectOnPlayer, L"Репутация")) repBonus += 2;
        if (WContains(m.effectOnPlayer, L"разблокирует") || WContains(m.effectOnPlayer, L"VIP")) eventBonus += 1;
    }

    if (bargainBonus > 0.35f) bargainBonus = 0.35f;
    if (fatigueMult < 0.65f) fatigueMult = 0.65f;
}

template<typename T>
T ClampT(T x, T a, T b) {
    return (x < a) ? a : (x > b) ? b : x;
}

static std::wstring PlayerBargainLine(BargainTactic tactic, bool isBuying) {
   
    switch (tactic) {
    case BargainTactic::BLUFF:
        return isBuying ? L"Слушай, это же завтра запретят. Я беру сейчас, но дешевле." : L"Такие медали уже на взлёте. Завтра дороже будет.";
    case BargainTactic::FLATTERY:
        return isBuying ? L"У вас глаз алмаз, видно — человек серьёзный. Сделаете цену по-доброму?" : L"Вы явно разбираетесь. Отдам вам, но по достойной цене.";
    case BargainTactic::THREAT:
        return isBuying ? L"Не перегибайте. Иначе тут быстро станет шумно." : L"Не зли меня. Я тоже умею разговаривать.";
    case BargainTactic::REASON:
        return isBuying ? L"По рынку это стоит меньше. Давайте по фактам, без сказок." : L"По рынку это стоит больше. Давайте честно, как взрослые.";
    case BargainTactic::PATIENCE:
        return isBuying ? L"Я подожду. Мне не горит. Цена сама опустится." : L"Я подожду. Найдётся тот, кто заплатит нормально.";
    default:
        return L"...";
    }
}

void Player::SfxVictory() {
    Beep(784, 90);    // G5
    Beep(1047, 90);   // C6
    Beep(1319, 180);  // E6
    Beep(1568, 140);  // G6
}

void Player::SfxDefeat() {
    Beep(622, 140);   // Eb5
    Beep(587, 140);   // D5
    Beep(523, 180);   // C5
    Beep(392, 260);   // G4
    Beep(196, 300);   // G3
}

void Player::SfxGameOver() {
    Beep(494, 160);   // B4
    Beep(466, 160);   // Bb4
    Beep(440, 180);   // A4
    Beep(349, 220);   // F4
    Beep(262, 420);   // C4
}

void Player::SfxUIConfirm() {
    Beep(988, 35);    // B5
    Beep(1319, 70);   // E6
}

void Player::SfxUIError() {
    Beep(220, 90);    // A3
    Beep(196, 90);    // G3
    Beep(220, 120);   // A3
}

void Player::Trade(Location& currentLocation) {
    if (inventory.IsEmpty()) {
        std::wcout << L"У тебя нет медалей для продажи.\n";
        return;
    }

    ShowInventory();

    std::wcout << L"Выбери медаль для продажи (1-" << inventory.Size() << L"): ";
    size_t choice = 0;
    std::wcin >> choice;

    if (choice < 1 || choice > inventory.Size()) {
        std::wcout << L"Неверный выбор!\n";
        return;
    }

    Medal selectedMedal = inventory.GetMedal(choice - 1);

    if (currentLocation.npcs.empty()) {
        std::wcout << L"Некому продавать здесь.\n";
        return;
    }

    NPC& npc = currentLocation.npcs[rand() % currentLocation.npcs.size()];

    ConsoleColors::SetColor(ConsoleColors::CYAN);
    std::wcout << L"\nПокупатель: " << npc.name << L" — " << npc.backstory << L"\n";
    ConsoleColors::Reset();

    // Игрок называет цену первым (без подсказок)
    int playerAsk = 0;
    std::wcout << L"Твоя цена (за сколько хочешь продать): ";
    std::wcin >> playerAsk;

    MedalManager manager;
    int market = manager.GetMarketValue(selectedMedal, reputation, false);

    // NPC отвечает своей ценой
    int npcOffer = (int)(market * currentLocation.priceModifier * 0.92f);
    if (npcOffer < (int)(selectedMedal.minPrice * currentLocation.priceModifier * 0.70f))
        npcOffer = (int)(selectedMedal.minPrice * currentLocation.priceModifier * 0.70f);
    if (npcOffer > (int)(selectedMedal.maxPrice * currentLocation.priceModifier * 1.05f))
        npcOffer = (int)(selectedMedal.maxPrice * currentLocation.priceModifier * 1.05f);

    // контр-оффер: между его и твоей
    if (playerAsk <= npcOffer) {
        // игрок просит мало — NPC не обязан повышать, но может чуть поднять
        npcOffer = npcOffer;
    }
    else {
        int delta = playerAsk - npcOffer;
        npcOffer = npcOffer + (int)(delta * 0.45f);
    }

    std::wcout << npc.name << L": Я дам " << npcOffer << L" руб.\n";
    std::wcout << L"Будешь торговаться? (1-Да, 0-Нет): ";

    int bargainChoice = 0;
    std::wcin >> bargainChoice;

    int finalPrice = npcOffer;

    if (bargainChoice == 1) {
        bool accepted = StartBargainDialogue(npc, selectedMedal, finalPrice, false, playerAsk);
        if (!accepted) {
            std::wcout << L"Сделка сорвалась.\n";
            return;
        }
    }

    std::wcout << L"Продать за " << finalPrice << L" руб.? (1-Да, 0-Нет): ";
    int confirm = 0;
    std::wcin >> confirm;
    if (confirm != 1) {
        std::wcout << L"Ты передумал.\n";
        return;
    }

    money += finalPrice;
    inventory.RemoveByIndex(choice - 1);

    // Репутация за честную продажу
    float ratio = (market > 0) ? (finalPrice / (float)market) : 1.0f;
    if (ratio >= 0.90f && ratio <= 1.35f) {
        reputation += 2;
        std::wcout << L"Репутация +2. Почему так: продажа выглядела честной, без перекоса.\n";
    }
    else if (ratio > 1.60f) {
        reputation -= 1;
        std::wcout << L"Репутация -1. Почему так: слишком завышенная цена портит слухи.\n";
    }

    std::wcout << L"Продано: " << selectedMedal.name << L" за " << finalPrice << L" руб.\n";
    std::wcout << L"Теперь у тебя " << money << L" руб.\n";
}
void Player::ShowNPCs(const std::vector<NPC>& npcs) const {
    if (npcs.empty()) {
        std::wcout << L"Здесь никого нет.\n";
        return;
    }

    std::wcout << L"\n=== NPC в этой локации ===\n";
    for (size_t i = 0; i < npcs.size(); ++i) {
        std::wcout << i + 1 << L". " << npcs[i].name
            << L" - " << npcs[i].backstory << L"\n";
    }
}

void Player::BuyFromNPC(Location& currentLocation) {
    if (currentLocation.npcs.empty()) {
        std::wcout << L"Здесь нет продавцов.\n";
        return;
    }

    std::wcout << L"\nВыберите NPC для покупки:\n";
    ShowNPCs(currentLocation.npcs);

    size_t npcChoice = 0;
    std::wcin >> npcChoice;
    if (npcChoice < 1 || npcChoice > currentLocation.npcs.size()) {
        std::wcout << L"Неверный выбор.\n";
        return;
    }

    NPC& npc = currentLocation.npcs[npcChoice - 1];

    // Всегда пополняем ассортимент (по ТЗ)
    if (npc.medalsForSale.size() < 3) npc.Restock(3 - (int)npc.medalsForSale.size());

    ConsoleColors::SetColor(ConsoleColors::CYAN);
    std::wcout << L"\n" << npc.name << L" — " << npc.backstory << L"\n";
    ConsoleColors::Reset();

    std::wcout << L"\nМедали у " << npc.name << L":\n";
    for (size_t i = 0; i < npc.medalsForSale.size(); ++i) {
        const auto& m = npc.medalsForSale[i];
        std::wcout << i + 1 << L". " << m.name << L" (" << m.condition << L")";
        if (!m.effectOnPlayer.empty()) std::wcout << L" | эффект: " << ShortEffect(m.effectOnPlayer);
        std::wcout << L"\n";
    }

    size_t medalChoice = 0;
    std::wcout << L"\nВыбери медаль (1-" << npc.medalsForSale.size() << L"): ";
    std::wcin >> medalChoice;
    if (medalChoice < 1 || medalChoice > npc.medalsForSale.size()) {
        std::wcout << L"Неверный выбор.\n";
        return;
    }

    Medal chosenMedal = npc.medalsForSale[medalChoice - 1];

    // Игрок называет цену первым (БЕЗ показа примерной цены)
    int playerOffer = 0;
    std::wcout << L"Твоя цена (сколько готов заплатить): ";
    std::wcin >> playerOffer;

    MedalManager manager;
    int market = manager.GetMarketValue(chosenMedal, reputation, false);

    // NPC отвечает своей ценой
    int npcPrice = (int)(market * currentLocation.priceModifier * 1.18f);
    if (npcPrice < (int)(chosenMedal.minPrice * currentLocation.priceModifier * 0.85f))
        npcPrice = (int)(chosenMedal.minPrice * currentLocation.priceModifier * 0.85f);
    if (npcPrice > (int)(chosenMedal.maxPrice * currentLocation.priceModifier * 1.20f))
        npcPrice = (int)(chosenMedal.maxPrice * currentLocation.priceModifier * 1.20f);

    // контр-цена: между твоей и его
    if (playerOffer >= npcPrice) {
        npcPrice = npcPrice; // он просто соглашается на свою цену
    }
    else {
        int delta = npcPrice - playerOffer;
        npcPrice = playerOffer + (int)(delta * 0.70f); // жёстко торгуется
    }

    std::wcout << npc.name << L": Моя цена — " << npcPrice << L" руб.\n";
    std::wcout << L"Будешь торговаться? (1-Да, 0-Нет): ";

    int bargainChoice = 0;
    std::wcin >> bargainChoice;

    int finalPrice = npcPrice;

    if (bargainChoice == 1) {
        bool accepted = StartBargainDialogue(npc, chosenMedal, finalPrice, true, playerOffer);
        if (!accepted) {
            std::wcout << L"Сделка сорвалась.\n";
            return;
        }
    }

    std::wcout << L"Купить за " << finalPrice << L" руб.? (1-Да, 0-Нет): ";
    int confirm = 0;
    std::wcin >> confirm;
    if (confirm != 1) {
        std::wcout << L"Ты передумал.\n";
        return;
    }

    if (money < finalPrice) {
        std::wcout << L"Недостаточно денег.\n";
        return;
    }

    // Сделка
    money -= finalPrice;
    inventory.Add(chosenMedal);
    npc.money += finalPrice;
    npc.medalsForSale.erase(npc.medalsForSale.begin() + (medalChoice - 1));

    if (npc.medalsForSale.size() < 3) npc.Restock(3 - (int)npc.medalsForSale.size());

    // Репутация за честность (цена близка к рынку)
    float ratio = (market > 0) ? (finalPrice / (float)market) : 1.0f;
    if (ratio >= 0.75f && ratio <= 1.10f) {
        reputation += 2;
        std::wcout << L"Репутация +2. Почему так: цена выглядела честной и рыночной.\n";
    }
    else if (ratio < 0.65f) {
        reputation -= 1;
        std::wcout << L"Репутация -1. Почему так: слишком жёсткий демпинг вызывает недоверие.\n";
    }

    std::wcout << L"Покупка успешна: " << chosenMedal.name << L"\n";
}

bool Player::StartBargainDialogue(NPC& npc, Medal& medal, int& currentPrice, bool isBuying, int playerFirstPrice) {
    float bargainBonus = 0.0f, fatigueMult = 1.0f;
    int repBonus = 0, eventBonus = 0;
    GetMedalPassives(*this, bargainBonus, fatigueMult, repBonus, eventBonus);

    // Иногда NPC требует "без тактик" (по ТЗ)
    bool manualOnly = false;
    if (npc.type == NPCType::COLLECTOR) manualOnly = (rand() % 100 < 35);
    else manualOnly = (rand() % 100 < 15);

    // NPC уже озвучил currentPrice (свою цену). Игрок уже озвучил playerFirstPrice.
    std::wcout << L"\n=== ТОРГ ===\n";
    std::wcout << L"Твоя цена: " << playerFirstPrice << L" руб.\n";
    std::wcout << npc.name << L": Моя цена: " << currentPrice << L" руб.\n";

    // Опция: сразу сбежать
    std::wcout << L"Отменить сделку и сбежать? (1-Да, 0-Нет): ";
    int flee = 0; std::wcin >> flee;
    if (flee == 1) {
        ConsoleColors::SetColor(ConsoleColors::RED);
        std::wcout << L"Ты сбежал. Почему так: ты прервал переговоры → репутация -1.\n";
        ConsoleColors::Reset();
        reputation -= 1;
        ChangeTrust(npc.name, -5);
        return false;
    }

    if (manualOnly) {
        ConsoleColors::SetColor(ConsoleColors::CYAN);
        std::wcout << npc.name << L": Без ваших приёмов. Назови финальную цену.\n";
        ConsoleColors::Reset();

        int finalOffer = 0;
        std::wcout << L"Твоя финальная цена: ";
        std::wcin >> finalOffer;

        // Простая справедливость: NPC принимает, если финальная ближе к его текущей
        int diff = (finalOffer > currentPrice) ? (finalOffer - currentPrice) : (currentPrice - finalOffer);
        int tolerance = (currentPrice * 0.12f > 150)
            ? (int)(currentPrice * 0.12f)
            : 150;

        if (diff <= tolerance) {
            currentPrice = finalOffer;
            ConsoleColors::SetColor(ConsoleColors::GREEN);
            std::wcout << L"Сделка принята.\nПочему так: финальная цена попала в допустимый диапазон.\n";
            ConsoleColors::Reset();

            reputation += 2; // честно договорились
            ChangeTrust(npc.name, +6);
            return true;
        }

        ConsoleColors::SetColor(ConsoleColors::RED);
        std::wcout << L"Отказ.\nПочему так: финальная цена слишком далека от условий NPC.\n";
        ConsoleColors::Reset();

        reputation -= 1;
        ChangeTrust(npc.name, -4);
        return false;
    }

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> roll(0.0f, 1.0f);

    int attempts = 0;
    const int maxAttempts = 4;

    while (true) {
        std::wcout << L"\nТекущая цена: " << currentPrice << L" руб.\n";
        std::wcout << L"Выбери тактику:\n";
        std::wcout << L"1. Блеф      (шанс: " << (int)(npc.CalculateTacticSuccessChance(BargainTactic::BLUFF, *this) * 100) << L"%)\n";
        std::wcout << L"2. Лесть     (шанс: " << (int)(npc.CalculateTacticSuccessChance(BargainTactic::FLATTERY, *this) * 100) << L"%)\n";
        std::wcout << L"3. Угроза    (шанс: " << (int)(npc.CalculateTacticSuccessChance(BargainTactic::THREAT, *this) * 100) << L"%)\n";
        std::wcout << L"4. Доводы    (шанс: " << (int)(npc.CalculateTacticSuccessChance(BargainTactic::REASON, *this) * 100) << L"%)\n";
        std::wcout << L"5. Терпение  (шанс: " << (int)(npc.CalculateTacticSuccessChance(BargainTactic::PATIENCE, *this) * 100) << L"%)\n";
        std::wcout << L"0. Принять текущую цену\n";
        std::wcout << L"Выбор: ";

        int tacticChoice = 0;
        std::wcin >> tacticChoice;

        if (tacticChoice == 0) break;
        if (tacticChoice < 1 || tacticChoice > 5) {
            std::wcout << L"Неверный выбор!\n";
            continue;
        }

        attempts++;
        BargainTactic tactic = static_cast<BargainTactic>(tacticChoice - 1);

        // цена усталости/голода с учётом медалей
        int baseFatigueCost = (tactic == BargainTactic::PATIENCE) ? 7 : (tactic == BargainTactic::THREAT ? 6 : 4);
        fatigue += (int)(baseFatigueCost * fatigueMult);
        hunger += 2;

        if (attempts >= maxAttempts) {
            std::wcout << L"\n" << npc.name << L": Хватит. Или берёшь, или уходи.\n";
            break;
        }

        float successChance = npc.CalculateTacticSuccessChance(tactic, *this);
        bool success = (roll(gen) < successChance);

        std::wcout << L"\nТы: " << npc.GetDialogResponse(tactic, false) << L"\n";
        std::wcout << npc.name << L": " << npc.GetDialogResponse(tactic, success) << L"\n";

        // Разные тактики — разные изменения цены (ощутимый разброс)
        float changeMin = 0.04f, changeMax = 0.10f;
        if (tactic == BargainTactic::REASON) { changeMin = 0.03f; changeMax = 0.07f; }
        if (tactic == BargainTactic::THREAT) { changeMin = 0.08f; changeMax = 0.18f; }
        if (tactic == BargainTactic::FLATTERY) { changeMin = 0.05f; changeMax = 0.11f; }
        if (tactic == BargainTactic::BLUFF) { changeMin = 0.06f; changeMax = 0.14f; }
        if (tactic == BargainTactic::PATIENCE) { changeMin = 0.02f; changeMax = 0.06f; }

        float swing = changeMin + (1.0f - successChance) * (changeMax - changeMin);

        if (success) {
            if (isBuying) currentPrice = (int)(currentPrice * (1.0f - swing));
            else         currentPrice = (int)(currentPrice * (1.0f + swing));

            // Бонус медалей на торг слегка усиливает успех
            currentPrice = isBuying
                ? (int)(currentPrice * (1.0f - bargainBonus * 0.35f))
                : (int)(currentPrice * (1.0f + bargainBonus * 0.20f));

            ChangeTrust(npc.name, +6);
            if (tactic == BargainTactic::REASON) reputation += 1;
            if (tactic == BargainTactic::THREAT) reputation -= 2; // угрозы вредят репутации
        }
        else {
            // провал зависит от тактики
            if (isBuying) {
                float backfire = (tactic == BargainTactic::THREAT) ? 0.08f : 0.03f;
                currentPrice = (int)(currentPrice * (1.0f + backfire));
            }
            // при продаже провал обычно "не двигает" цену, но портит доверие/репутацию
            ChangeTrust(npc.name, -6);
            reputation -= (tactic == BargainTactic::THREAT ? 2 : 1);
        }

        std::wcout << L"Почему так: шанс зависел от типа NPC, твоей репутации/доверия и выбранной тактики.\n";
        std::wcout << L"Твоя репутация теперь: " << reputation << L"\n";
    }

    std::wcout << L"\nПринять цену " << currentPrice << L" руб.? (1-Да, 0-Нет): ";
    int finalChoice = 0;
    std::wcin >> finalChoice;

    if (finalChoice == 1) {
        ConsoleColors::SetColor(ConsoleColors::GREEN);
        std::wcout << L"Сделка заключена.\nПочему так: ты принял финальные условия.\n";
        ConsoleColors::Reset();
        ChangeTrust(npc.name, +4);
        return true;
    }

    ConsoleColors::SetColor(ConsoleColors::RED);
    std::wcout << L"Торг прекращён.\nПочему так: ты отказался от финальной цены.\n";
    ConsoleColors::Reset();

    reputation -= 1;
    ChangeTrust(npc.name, -3);
    return false;
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

void Player::InitPrevStats() {
    prevMoney = money;
    prevHunger = hunger;
    prevFatigue = fatigue;
    prevReputation = reputation;
}

void Player::ShowChangedStats() {
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
}

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

int Player::GetTrust(const std::wstring& npcName) const {
    auto it = npcTrust.find(npcName);
    if (it == npcTrust.end()) return 0;
    return it->second;
}

void Player::ChangeTrust(const std::wstring& npcName, int delta) {
    int cur = GetTrust(npcName);
    cur = ClampT(cur + delta, -100, 100);
    npcTrust[npcName] = cur;
}