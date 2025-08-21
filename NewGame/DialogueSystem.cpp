#include "DialogueSystem.h"
#include <iostream>
#include <locale>
#include <algorithm>
#include <random>
#include <chrono>

static const std::vector<DialogueOption> playerDialogues = {
    // BLUFF
    {L"Да пошёл ты… я же просто смотрю на твою медаль, а не на твою душу.", BargainTactic::BLUFF, 10, 0},
    {L"Если купишь у меня — я не отвечаю за последствия, понял, чёрт побери?", BargainTactic::BLUFF, 15, 0},
    {L"Я могу уйти, но тебе станет скучно, блин, правда?", BargainTactic::BLUFF, 8, 0},
    {L"Смотри, я шучу, но это серьёзно… почти.", BargainTactic::BLUFF, 12, 0},
    {L"Ты правда веришь, что я за медалью? Ха-ха, бред какой-то.", BargainTactic::BLUFF, 5, 0},

    // FLATTERY
    {L"Ты, конечно, охренительно выглядишь, просто бомба.", BargainTactic::FLATTERY, 10, 0},
    {L"С таким умом ты, наверное, в детстве убирала чужие ошибки и радовалась.", BargainTactic::FLATTERY, 8, 0},
    {L"Ох, да, твоя хитрость как нож… прям в жопу попадёшь, если не осторожен.", BargainTactic::FLATTERY, 12, 0},
    {L"Я почти тронут. Почти. Но это не точно.", BargainTactic::FLATTERY, 6, 0},
    {L"Твоя харизма… ну, короче, какая-то чертовщина.", BargainTactic::FLATTERY, 9, 0},

    // THREAT
    {L"Попробуй меня кинуть — и эта медаль превратится в твою долбанную проблему.", BargainTactic::THREAT, 15, 0},
    {L"Я знаю всех, кто доверчивый… и кто потом горит. Так что будь аккуратна.", BargainTactic::THREAT, 12, 0},
    {L"Если не уступишь, я расскажу, как ты пытаешься наивных разводить… блин.", BargainTactic::THREAT, 10, 0},
    {L"Я не люблю недоговорённостей. Честно.", BargainTactic::THREAT, 8, 0},
    {L"Думаешь, это торг? Нет, это игра в кошки-мышки, сука.", BargainTactic::THREAT, 14, 0},

    // REASON
    {L"Слушай, если мы умны, выгоднее компромисс. Я не против, чёрт возьми.", BargainTactic::REASON, 10, 0},
    {L"Цена завышена. Я могу показать графики. Да, графики!", BargainTactic::REASON, 12, 0},
    {L"Сохраним лицо обоих. Или нет — мне похер.", BargainTactic::REASON, 8, 0},
    {L"Это не личное. Это бизнес, мать его.", BargainTactic::REASON, 11, 0},
    {L"Давай договоримся, а если нет — ну что ж… жизнь продолжается.", BargainTactic::REASON, 9, 0},

    // PATIENCE
    {L"Сначала кофе, потом торг. Или наоборот. Мне пофиг.", BargainTactic::PATIENCE, 7, 0},
    {L"Я могу ждать. И ждать… и ждать. Пошли все к чёрту.", BargainTactic::PATIENCE, 12, 0},
    {L"Посмотрим, кто первым моргнёт. Я не моргну. Точка.", BargainTactic::PATIENCE, 10, 0},
    {L"Не спешим. Время дорого, как твоя чёртова медаль.", BargainTactic::PATIENCE, 8, 0},
    {L"Давай медленно, красиво… и без смысла. Просто болтаем.", BargainTactic::PATIENCE, 6, 0}
};

int DialogueSystem::ReadInt() {
    int v = 0;
    if (!(std::wcin >> v)) {
        std::wcin.clear();
        std::wstring tmp;
        std::getline(std::wcin, tmp);
        return 0;
    }
    return v;
}

bool DialogueSystem::CheckSuccess(int chance) {
    // Простой и безопасный способ проверки успеха
    static unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    static std::mt19937 generator(seed);

    // Генерируем число от 0 до 99
    int randomValue = generator() % 100;
    return randomValue < chance;
}

std::wstring DialogueSystem::GetNPCResponse(BargainTactic tactic, bool success, int round, const NPC& npc) {
    switch (tactic) {
    case BargainTactic::BLUFF:
        return success ? L"(Блеф) Ха! Ну ладно — почти поверил. Дам по-человечески." :
            L"Да ты гонишь. Никаких сказок, держи свой прайс при себе.";
    case BargainTactic::FLATTERY:
        return success ? L"(Лесть) Лесть — твоё второе имя. Хорошо, уступлю чуть-чуть." :
            L"Слащаво. Мне нравится, но деньги любят факты, а не песни.";
    case BargainTactic::THREAT:
        return success ? L"(Угроза) Окей, понятно. Никто не хочет проблем. Давай мирно." :
            L"Угрожать — плохая стратегия, если у тебя нет веса за спине.";
    case BargainTactic::REASON:
        return success ? L"(Разум) Таблички и графики — моя больная точка. Ладно, логика есть." :
            L"Цифры красивые, но факты — важнее. Вернись с доказательствами.";
    case BargainTactic::PATIENCE:
        return success ? L"(Терпение) Ты терпелив, я вижу. Хорошо, я уступлю, лишь бы ты ушёл довольным." :
            L"Ждать можно вечно, но медаль остаётся той же. Не торгуйтесь со временем.";
    default:
        return L"...";
    }
}

int DialogueSystem::CalculateChance(const DialogueOption& opt, const Player& player, const NPC& npc, const Medal& medal, int round) {
    float npcBase = 50.0f;
    try {
        npcBase = npc.CalculateTacticSuccessChance(opt.tactic, player) * 100.0f;
    }
    catch (...) {
        npcBase = 50.0f;
    }

    int chance = static_cast<int>(npcBase) + opt.successModifier + (player.reputation * 1);

    if (medal.effect == MedalEffect::BargainBonus) {
        chance += medal.effectValue;
    }

    chance -= round * 6;

    if (chance < 5) chance = 5;
    if (chance > 95) chance = 95;
    return chance;
}

void DialogueSystem::StartMultiRoundDialogue(Player& player, NPC& npc, size_t playerMedalIndex, int startingPrice) {
    if (playerMedalIndex >= player.inventory.Size()) {
        std::wcout << L"Ошибка: неверный индекс медали.\n";
        return;
    }

    Medal medal = player.inventory.GetMedal(playerMedalIndex);
    int currentPrice = startingPrice;
    int round = 0;
    const int maxRounds = 6;
    std::vector<size_t> usedOptions;

    std::wcout << L"Начинается торг за: " << medal.name << L" (состояние: " << medal.condition << L")\n";

    while (round < maxRounds) {
        std::wcout << L"\n--- Раунд " << (round + 1) << L" ---\n";
        std::wcout << L"Текущая цена: " << currentPrice << L"    Репутация: " << player.reputation << L"\n";

        // доступные опции
        std::vector<size_t> available;
        for (size_t i = 0; i < playerDialogues.size(); ++i) {
            const auto& opt = playerDialogues[i];
            if (std::find(usedOptions.begin(), usedOptions.end(), i) != usedOptions.end()) continue;
            if (player.reputation < opt.unlockReputation) continue;
            available.push_back(i);
        }

        if (available.empty()) {
            std::wcout << L"Нет больше вариантов — торг затухает.\n";
            break;
        }

        // показываем варианты
        std::wcout << L"Выберите фразу:\n";
        size_t showCount = std::min<size_t>(6, available.size());
        for (size_t i = 0; i < showCount; ++i) {
            size_t idx = available[i];
            std::wcout << (i + 1) << L") " << playerDialogues[idx].text << L"\n";
        }
        std::wcout << (showCount + 1) << L") " << L"Другой ход (завершить торг или изменить стратегию)\n";

        int choice = ReadInt();
        if (choice < 1 || choice > static_cast<int>(showCount + 1)) {
            std::wcout << L"Неверный ввод — пропуск.\n";
            break;
        }

        if (choice == static_cast<int>(showCount + 1)) {
            std::wcout << L"Вы решили прекратить торг.\n";
            break;
        }

        size_t chosenIndex = available[static_cast<size_t>(choice - 1)];
        const DialogueOption& chosen = playerDialogues[chosenIndex];
        usedOptions.push_back(chosenIndex);

        int chance = CalculateChance(chosen, player, npc, medal, round);
        bool success = CheckSuccess(chance);

        std::wcout << L"\nВы: " << chosen.text << L"\n";
        std::wstring npcResp = GetNPCResponse(chosen.tactic, success, round, npc);
        std::wcout << npc.name << L": " << npcResp << L"\n";

        if (success) {
            int priceDrop = 50 + chosen.successModifier * 5;
            currentPrice = std::max(currentPrice - priceDrop, medal.minPrice);
            player.reputation += 1 + (chosen.tactic == BargainTactic::FLATTERY ? 1 : 0);
            std::wcout << L"Ход удался. Цена упала на " << priceDrop << L"\n";

            if (medal.effect == MedalEffect::MoneyBonus) {
                player.money += medal.effectValue;
                std::wcout << L"Бонус от медали: +" << medal.effectValue << L"\n";
            }
        }
        else {
            player.reputation -= 1;
            int priceRise = 20;
            currentPrice += priceRise;
            std::wcout << L"Провал. Репутация -1. Цена выросла на " << priceRise << L"\n";
        }

        // автоматическая продажа
        if (currentPrice <= medal.minPrice) {
            std::wcout << L"\nЦена достигла минимума. NPC соглашается купить медаль за " << currentPrice << L"\n";
            player.money += currentPrice;
            player.inventory.RemoveByIndex(playerMedalIndex);
            npc.AddMedal(medal);
            std::wcout << L"Медаль продана. Ваш баланс: " << player.money << L"\n";
            return;
        }

        // предложение продать сейчас
        std::wcout << L"\nХотите продать сейчас за " << currentPrice << L"? (1=Да / любая другая=Продолжать)\n";
        int sellNow = ReadInt();
        if (sellNow == 1) {
            player.money += currentPrice;
            player.inventory.RemoveByIndex(playerMedalIndex);
            npc.AddMedal(medal);
            std::wcout << L"Медаль продана. Ваш баланс: " << player.money << L"\n";
            return;
        }

        ++round;
    }

    std::wcout << L"\nТорг закончился. Медаль осталась у вас.\n";
}