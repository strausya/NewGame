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

    // ���� ������ ������� �� gullibility NPC � ��������� ������
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
        L"� ������, ����� ������ ����� ������ �� �������...",
        L"������ ����� ���� - ��� �� ����� ��������, ��?",
        L"��� ���������� ����� �� ������� � ������ �����"
    };

    dialogLines[BargainTactic::FLATTERY] = {
        L"����� ������ ��� �� ����� ����� ��������� ��������!",
        L"��� �������� � ����� ������ - ������ ����!",
        L"�� �� ��������� ���� � ������ �����������!"
    };

    dialogLines[BargainTactic::THREAT] = {
        L"������, � ���� � � ������� ���������...",
        L"��� ������ �� ������ ����� ����...",
        L"�� ���� �� ������ �������?"
    };

    dialogLines[BargainTactic::REASON] = {
        L"������� ���������� ���������...",
        L"�� �������� ��������� ��� �����...",
        L"� ��������� ������������ ����"
    };

    dialogLines[BargainTactic::PATIENCE] = {
        L"� ����� ���������...",
        L"������� �� �������...",
        L"����� �������� �� ���"
    };
}

std::wstring NPC::GetDialogResponse(BargainTactic tactic, bool success) {
    static std::map<BargainTactic, std::vector<std::wstring>> successResponses = {
        {BargainTactic::BLUFF, {L"�����, ������...", L"������, ������ ��� ����!", L"�� ������� ����������!"}},
        {BargainTactic::FLATTERY, {L"�� ����, � ����������� �������!", L"��� ������ ������� ������ ����������!", L"����� - ���� ������!"}},
        {BargainTactic::THREAT, {L"�� ���� ����������, � ��������!", L"�����, �� ����� ���������!", L"�� ������� ���� �����..."}},
        {BargainTactic::REASON, {L"���� ������ ����������", L"�������� ��������", L"������������ �����������"}},
        {BargainTactic::PATIENCE, {L"�� ��������, � �������", L"�����, �� ���� �����������", L"�� �������� ��� ����"}}
    };

    static std::map<BargainTactic, std::vector<std::wstring>> failResponses = {
        {BargainTactic::BLUFF, {L"� �� �����!", L"�� ����� ����!", L"�������� ��� ���"}},
        {BargainTactic::FLATTERY, {L"����� �� �������!", L"������� ��������!", L"� �� ��������"}},
        {BargainTactic::THREAT, {L"�� ��� �� �������!", L"��� �������?", L"������ �����"}},
        {BargainTactic::REASON, {L"���� ������ ��������", L"�� �����������", L"� �� ��������"}},
        {BargainTactic::PATIENCE, {L"� ���� ���� ���� �����", L"�� ������!", L"���"}}
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