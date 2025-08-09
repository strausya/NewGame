#include "NPC.h"
#include "Player.h"
#include <algorithm>
#include <random>
#include <cmath>

NPC::NPC(const std::wstring& name, const std::wstring& backstory,
    float bargainDifficulty, float gullibility, int money) :
    name(name), backstory(backstory),
    bargainDifficulty(bargainDifficulty),
    gullibility(gullibility), money(money) {
    InitializeDialogs();
}

void NPC::AddMedal(const Medal& medal) {
    medalsForSale.push_back(medal);
}

bool NPC::TryToCheat(Player& player, const Medal& medal) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    // Шанс успеха зависит от gullibility NPC и репутации игрока
    float successChance = gullibility * (1.0f - player.reputation * 0.01f);
    if (dis(gen) < successChance) {
        player.reputation -= 5;
        return true;
    }
    else {
        player.reputation -= 15;
        return false;
    }
}
int NPC::Bargain(int initialPrice, float playerSkill) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    float bargainEffect = playerSkill * (1.0f - bargainDifficulty);
    float discount = std::fmin(0.3f, bargainEffect * 0.3f);  // Fixed clamp function

    return static_cast<int>(initialPrice * (1.0f - discount));
}

void NPC::InitializeDialogs() {
    dialogLines[BargainTactic::BLUFF] = {
        L"Я слышал, такие медали скоро изымут из оборота...",
        L"Только между нами - это же явная подделка, да?",
        L"Мне предлагали такую же дешевле в другом месте"
    };

    dialogLines[BargainTactic::FLATTERY] = {
        L"Такой знаток как вы сразу видит настоящую ценность!",
        L"Для человека с вашим вкусом - особая цена!",
        L"Вы же понимаете толк в редких экземплярах!"
    };

    dialogLines[BargainTactic::THREAT] = {
        L"Знаете, я могу и в полицию позвонить...",
        L"Мои друзья не оценят такой цены...",
        L"Вы ведь не хотите проблем?"
    };

    dialogLines[BargainTactic::REASON] = {
        L"Давайте рассуждать логически...",
        L"По рыночной стоимости это стоит...",
        L"Я предлагаю справедливую цену"
    };

    dialogLines[BargainTactic::PATIENCE] = {
        L"Я готов подождать...",
        L"Давайте не спешить...",
        L"Время работает на нас"
    };
}

std::wstring NPC::GetDialogResponse(BargainTactic tactic, bool success) {
    static std::map<BargainTactic, std::vector<std::wstring>> successResponses = {
        {BargainTactic::BLUFF, {L"Ладно, убедил...", L"Хорошо, только для тебя!", L"Ты опасный собеседник!"}},
        {BargainTactic::FLATTERY, {L"Ты прав, я великодушен сегодня!", L"Для такого клиента сделаю исключение!", L"Лесть - твое оружие!"}},
        {BargainTactic::THREAT, {L"Не надо нервничать, я согласен!", L"Ладно, не стоит ссориться!", L"Ты выиграл этот раунд..."}},
        {BargainTactic::REASON, {L"Твоя логика безупречна", L"Разумный аргумент", L"Справедливое предложение"}},
        {BargainTactic::PATIENCE, {L"Ты терпелив, я уступаю", L"Ладно, не буду торговаться", L"Ты заслужил эту цену"}}
    };

    static std::map<BargainTactic, std::vector<std::wstring>> failResponses = {
        {BargainTactic::BLUFF, {L"Я не дурак!", L"Не смеши меня!", L"Попробуй еще раз"}},
        {BargainTactic::FLATTERY, {L"Лесть не поможет!", L"Слишком очевидно!", L"Я не поддамся"}},
        {BargainTactic::THREAT, {L"Ты мне не угрожай!", L"Сам боишься?", L"Пустые слова"}},
        {BargainTactic::REASON, {L"Твоя логика ошибочна", L"Не убедительно", L"Я не согласен"}},
        {BargainTactic::PATIENCE, {L"У меня тоже есть время", L"Не выйдет!", L"Нет"}}
    };

    auto& responses = success ? successResponses[tactic] : failResponses[tactic];
    return responses[rand() % responses.size()];
}

float NPC::CalculateTacticSuccessChance(BargainTactic tactic, const Player& player) {
    float baseChance = 0.3f;

    switch (tactic) {
    case BargainTactic::BLUFF:
        baseChance += player.reputation * 0.001f;
        break;
    case BargainTactic::FLATTERY:
        baseChance += (1.0f - gullibility) * 0.2f;
        break;
    case BargainTactic::THREAT:
        baseChance -= player.reputation * 0.002f;
        break;
    case BargainTactic::REASON:
        baseChance += 0.1f;
        break;
    case BargainTactic::PATIENCE:
        baseChance += 0.05f;
        break;
    }

    // Manual clamp for compatibility
    if (baseChance < 0.1f) return 0.1f;
    if (baseChance > 0.9f) return 0.9f;
    return baseChance;
}