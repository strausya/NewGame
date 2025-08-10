#include "NPC.h"
#include "Player.h"
#include <algorithm>
#include <random>
#include <cmath>



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
    float discount = std::fmin(0.3f, bargainEffect * 0.3f); 

    return static_cast<int>(initialPrice * (1.0f - discount));
}

void NPC::InitializeDialogs() {
    dialogLines.clear();

    std::map<BargainTactic, std::vector<std::wstring>> baseDialogs = {
        {BargainTactic::BLUFF, {
            L"� ������, ����� ������ ����� ������ �� �������...",
            L"������ ����� ���� - ��� �� ����� ��������, ��?",
            L"��� ���������� ����� �� ������� � ������ �����"
        }},
        {BargainTactic::FLATTERY, {
            L"����� ������ ��� �� ����� ����� ��������� ��������!",
            L"��� �������� � ����� ������ - ������ ����!",
            L"�� �� ��������� ���� � ������ �����������!"
        }},
        {BargainTactic::THREAT, {
            L"������, � ���� � � ������� ���������...",
            L"��� ������ �� ������ ����� ����...",
            L"�� ���� �� ������ �������?"
        }},
        {BargainTactic::REASON, {
            L"������� ���������� ���������...",
            L"�� �������� ��������� ��� �����...",
            L"� ��������� ������������ ����"
        }},
        {BargainTactic::PATIENCE, {
            L"� ����� ���������...",
            L"������� �� �������...",
            L"����� �������� �� ���"
        }}
    };

    // Fix switch statement
    switch (type) {
    case NPCType::TRADER:
        baseDialogs[BargainTactic::THREAT] = {
            L"� ���� ���� �������� � �������...",
            L"� ���� ������ �� ����� ���!",
            L"�� ������, � ��� ��������������?"
        };
        baseDialogs[BargainTactic::FLATTERY] = {
            L"�� �����, ������",
            L"������ ����!",
            L"����� �� �������"
        };
        break;

    case NPCType::COLLECTOR:
        baseDialogs[BargainTactic::REASON] = {
            L"���������� ��������...",
            L"�� �������� ��� ������ �����...",
            L"���� ������ ����������� ��������"
        };
        baseDialogs[BargainTactic::THREAT] = {
            L"� ������� ������!",
            L"��� �����������!",
            L"�� ���������� �������"
        };
        break;

    case NPCType::VETERAN:
        baseDialogs[BargainTactic::FLATTERY] = {
            L"� ���� ����� ������ ��������� ������!",
            L"�������� ������ �� �������� �������� ���� �������",
            L"�� ������!"
        };
        baseDialogs[BargainTactic::THREAT] = {
            L"� ������ �����, ���� �� ���������!",
            L"� ��� ���� ����� ����� �� �������",
            L"� ���� ��-����������� ������!"
        };
        break;

    case NPCType::HOBBYIST:
        baseDialogs[BargainTactic::BLUFF] = {
            L"������? � �� ����...",
            L"��������?",
            L"��, � � �����..."
        };
        baseDialogs[BargainTactic::FLATTERY] = {
            L"�� �����, � ������� ������������!",
            L"�������, � ��������!",
            L"�� ������, ��� ��� �������!"
        };
        break;
    }
}

std::wstring NPC::GetDialogResponse(BargainTactic tactic, bool success) {
    static std::map<NPCType, std::map<BargainTactic, std::vector<std::wstring>>> successResponses = {
        {NPCType::TRADER, {
            {BargainTactic::BLUFF, {L"�����, ������...", L"������, ������ ��� ����!", L"�� ������� ����������!"}},
            {BargainTactic::FLATTERY, {L"�� �����, �� �����", L"������ ���� �������", L"����� �� ����"}},
            {BargainTactic::THREAT, {L"�� ���� ����������", L"�����, �� ����� ���������", L"�� ������� ���� �����..."}},
            {BargainTactic::REASON, {L"������ ���� ������", L"�������� ��������", L"������������ �����������"}},
            {BargainTactic::PATIENCE, {L"����� - ������", L"�����, �� ���� �����������", L"�� �������� ��� ����"}}
        }},
        {NPCType::COLLECTOR, {
            {BargainTactic::BLUFF, {L"���������� ����", L"������, � ��������", L"�� ������ ����"}},
            {BargainTactic::FLATTERY, {L"�� �����, � �������", L"��� ������ ������� - ������ ����", L"����� - ���� ������!"}},
            {BargainTactic::THREAT, {L"��� �����������!", L"� ������� ������!", L"�� ���������� �������"}},
            {BargainTactic::REASON, {L"���� ������ ����������", L"�������� ��������", L"������������ �����������"}},
            {BargainTactic::PATIENCE, {L"�� ��������, � �������", L"�����, �� ���� �����������", L"�� �������� ��� ����"}}
        }},
        {NPCType::VETERAN, {
            {BargainTactic::BLUFF, {L"� ���� ����� ��� �� �����!", L"�����, ������ �������", L"�� ������� ����������!"}},
            {BargainTactic::FLATTERY, {L"��� �������� ������ ����������!", L"������� �� ������ �����", L"�� ������!"}},
            {BargainTactic::THREAT, {L"� ������ �����!", L"���� �� ���������!", L"�� ������� ���� �����..."}},
            {BargainTactic::REASON, {L"� ���� ����� ��� �� �������", L"�������� ��������", L"������������ �����������"}},
            {BargainTactic::PATIENCE, {L"�� ����� �����...", L"�����, �� ���� �����������", L"�� �������� ��� ����"}}
        }},
        {NPCType::HOBBYIST, {
            {BargainTactic::BLUFF, {L"��, ������?", L"������, � ��������", L"�� ������ ����"}},
            {BargainTactic::FLATTERY, {L"�������!", L"� ��������!", L"�� ������, ��� ��� �������!"}},
            {BargainTactic::THREAT, {L"�... � ������� ����...", L"�� ����...", L"����������, �� ����..."}},
            {BargainTactic::REASON, {L"� � ��������� �����...", L"�������� ��������", L"������������ �����������"}},
            {BargainTactic::PATIENCE, {L"� �������...", L"�����, �� ���� �����������", L"�� �������� ��� ����"}}
        }}
    };

    static std::map<NPCType, std::map<BargainTactic, std::vector<std::wstring>>> failResponses = {
        {NPCType::TRADER, {
            {BargainTactic::BLUFF, {L"�� ����� ��� �� ���!", L"�� ���� �����������?", L"��-��, ���������..."}},
            {BargainTactic::FLATTERY, {L"� ���� ���, ����� �� ��������?", L"����� �����", L"�� �������"}},
            {BargainTactic::THREAT, {L"������ ��� �����������", L"������ ��������!", L"������"}},
            {BargainTactic::REASON, {L"������ ��� �� �����������", L"�� ������", L"��� ���� ��������"}},
            {BargainTactic::PATIENCE, {L"� � �� ���� �������", L"������?", L"�� ������"}}
        }},
        {NPCType::COLLECTOR, {
            {BargainTactic::BLUFF, {L"������� ��������� �����������", L"� ���� ���� ���� ������", L"�������� ��� ���"}},
            {BargainTactic::FLATTERY, {L"�������, �� ������ �� ����� �� ���������", L"������� ��������", L"�� ��������"}},
            {BargainTactic::THREAT, {L"��� ������? � �����", L"������, �� ���", L"�� ���������� �����"}},
            {BargainTactic::REASON, {L"��������� �������", L"����� �����", L"�������������"}},
            {BargainTactic::PATIENCE, {L"� ������������, � ��� ������", L"������ � ���� ������", L"�� ���������"}}
        }},
        {NPCType::VETERAN, {
            {BargainTactic::BLUFF, {L"� ����� ����� ��� �� �����", L"�� ����� ����������", L"�������� �����"}},
            {BargainTactic::FLATTERY, {L"����� ������� ���", L"�� ���������", L"�������, �� ���"}},
            {BargainTactic::THREAT, {L"� � ������ �����", L"�� �����", L"������ �� ����"}},
            {BargainTactic::REASON, {L"������ ��� �� �������", L"�������� ������", L"�� ��������"}},
            {BargainTactic::PATIENCE, {L"�� ����� �����", L"����� � ���� ����", L"��� �������� ���� �� ������"}}
        }},
        {NPCType::HOBBYIST, {
            {BargainTactic::BLUFF, {L"��������? ��� ������?", L"� �� ������...", L"������ �������"}},
            {BargainTactic::FLATTERY, {L"��, �������", L"��� �������", L"����� ����"}},
            {BargainTactic::THREAT, {L"�... � �����", L"�� ���� ���", L"����������, �� �����"}},
            {BargainTactic::REASON, {L"� ��������� ��-������� �����", L"�� ����...", L"�����������"}},
            {BargainTactic::PATIENCE, {L"� �������", L"�� �����...", L"����� ���?"}}
        }}
    };

    const auto& responsesMap = success ? successResponses : failResponses;
    const auto& responses = responsesMap.at(type).at(tactic);

    if (responses.empty()) {
        return L"...";
    }
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