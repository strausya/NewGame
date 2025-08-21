#include "DialogueSystem.h"
#include <iostream>
#include <locale>
#include <algorithm>
#include <random>
#include <chrono>

static const std::vector<DialogueOption> playerDialogues = {
    // BLUFF
    {L"�� ����� ��� � �� ������ ������ �� ���� ������, � �� �� ���� ����.", BargainTactic::BLUFF, 10, 0},
    {L"���� ������ � ���� � � �� ������� �� �����������, �����, ���� ������?", BargainTactic::BLUFF, 15, 0},
    {L"� ���� ����, �� ���� ������ ������, ����, ������?", BargainTactic::BLUFF, 8, 0},
    {L"������, � ����, �� ��� �������� �����.", BargainTactic::BLUFF, 12, 0},
    {L"�� ������ ������, ��� � �� �������? ��-��, ���� �����-��.", BargainTactic::BLUFF, 5, 0},

    // FLATTERY
    {L"��, �������, ������������ ���������, ������ �����.", BargainTactic::FLATTERY, 10, 0},
    {L"� ����� ���� ��, ��������, � ������� ������� ����� ������ � ����������.", BargainTactic::FLATTERY, 8, 0},
    {L"��, ��, ���� �������� ��� ��� ���� � ���� �������, ���� �� ���������.", BargainTactic::FLATTERY, 12, 0},
    {L"� ����� ������. �����. �� ��� �� �����.", BargainTactic::FLATTERY, 6, 0},
    {L"���� �������� ��, ������, �����-�� ����������.", BargainTactic::FLATTERY, 9, 0},

    // THREAT
    {L"�������� ���� ������ � � ��� ������ ����������� � ���� ��������� ��������.", BargainTactic::THREAT, 15, 0},
    {L"� ���� ����, ��� ���������� � ��� ����� �����. ��� ��� ���� ���������.", BargainTactic::THREAT, 12, 0},
    {L"���� �� ��������, � ��������, ��� �� ��������� ������� ���������� ����.", BargainTactic::THREAT, 10, 0},
    {L"� �� ����� ����������������. ������.", BargainTactic::THREAT, 8, 0},
    {L"�������, ��� ����? ���, ��� ���� � �����-�����, ����.", BargainTactic::THREAT, 14, 0},

    // REASON
    {L"������, ���� �� ����, �������� ����������. � �� ������, ���� ������.", BargainTactic::REASON, 10, 0},
    {L"���� ��������. � ���� �������� �������. ��, �������!", BargainTactic::REASON, 12, 0},
    {L"�������� ���� �����. ��� ��� � ��� �����.", BargainTactic::REASON, 8, 0},
    {L"��� �� ������. ��� ������, ���� ���.", BargainTactic::REASON, 11, 0},
    {L"����� �����������, � ���� ��� � �� ��� � ����� ������������.", BargainTactic::REASON, 9, 0},

    // PATIENCE
    {L"������� ����, ����� ����. ��� ��������. ��� �����.", BargainTactic::PATIENCE, 7, 0},
    {L"� ���� �����. � ������ � �����. ����� ��� � �����.", BargainTactic::PATIENCE, 12, 0},
    {L"���������, ��� ������ ������. � �� ������. �����.", BargainTactic::PATIENCE, 10, 0},
    {L"�� ������. ����� ������, ��� ���� ������� ������.", BargainTactic::PATIENCE, 8, 0},
    {L"����� ��������, ������� � ��� ������. ������ �������.", BargainTactic::PATIENCE, 6, 0}
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
    // ������� � ���������� ������ �������� ������
    static unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    static std::mt19937 generator(seed);

    // ���������� ����� �� 0 �� 99
    int randomValue = generator() % 100;
    return randomValue < chance;
}

std::wstring DialogueSystem::GetNPCResponse(BargainTactic tactic, bool success, int round, const NPC& npc) {
    switch (tactic) {
    case BargainTactic::BLUFF:
        return success ? L"(����) ��! �� ����� � ����� �������. ��� ��-�����������." :
            L"�� �� ������. ������� ������, ����� ���� ����� ��� ����.";
    case BargainTactic::FLATTERY:
        return success ? L"(�����) ����� � ��� ������ ���. ������, ������� ����-����." :
            L"�������. ��� ��������, �� ������ ����� �����, � �� �����.";
    case BargainTactic::THREAT:
        return success ? L"(������) ����, �������. ����� �� ����� �������. ����� �����." :
            L"�������� � ������ ���������, ���� � ���� ��� ���� �� �����.";
    case BargainTactic::REASON:
        return success ? L"(�����) �������� � ������� � ��� ������� �����. �����, ������ ����." :
            L"����� ��������, �� ����� � ������. ������� � ����������������.";
    case BargainTactic::PATIENCE:
        return success ? L"(��������) �� ��������, � ����. ������, � �������, ���� �� �� ���� ���������." :
            L"����� ����� �����, �� ������ ������� ��� ��. �� ���������� �� ��������.";
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
        std::wcout << L"������: �������� ������ ������.\n";
        return;
    }

    Medal medal = player.inventory.GetMedal(playerMedalIndex);
    int currentPrice = startingPrice;
    int round = 0;
    const int maxRounds = 6;
    std::vector<size_t> usedOptions;

    std::wcout << L"���������� ���� ��: " << medal.name << L" (���������: " << medal.condition << L")\n";

    while (round < maxRounds) {
        std::wcout << L"\n--- ����� " << (round + 1) << L" ---\n";
        std::wcout << L"������� ����: " << currentPrice << L"    ���������: " << player.reputation << L"\n";

        // ��������� �����
        std::vector<size_t> available;
        for (size_t i = 0; i < playerDialogues.size(); ++i) {
            const auto& opt = playerDialogues[i];
            if (std::find(usedOptions.begin(), usedOptions.end(), i) != usedOptions.end()) continue;
            if (player.reputation < opt.unlockReputation) continue;
            available.push_back(i);
        }

        if (available.empty()) {
            std::wcout << L"��� ������ ��������� � ���� ��������.\n";
            break;
        }

        // ���������� ��������
        std::wcout << L"�������� �����:\n";
        size_t showCount = std::min<size_t>(6, available.size());
        for (size_t i = 0; i < showCount; ++i) {
            size_t idx = available[i];
            std::wcout << (i + 1) << L") " << playerDialogues[idx].text << L"\n";
        }
        std::wcout << (showCount + 1) << L") " << L"������ ��� (��������� ���� ��� �������� ���������)\n";

        int choice = ReadInt();
        if (choice < 1 || choice > static_cast<int>(showCount + 1)) {
            std::wcout << L"�������� ���� � �������.\n";
            break;
        }

        if (choice == static_cast<int>(showCount + 1)) {
            std::wcout << L"�� ������ ���������� ����.\n";
            break;
        }

        size_t chosenIndex = available[static_cast<size_t>(choice - 1)];
        const DialogueOption& chosen = playerDialogues[chosenIndex];
        usedOptions.push_back(chosenIndex);

        int chance = CalculateChance(chosen, player, npc, medal, round);
        bool success = CheckSuccess(chance);

        std::wcout << L"\n��: " << chosen.text << L"\n";
        std::wstring npcResp = GetNPCResponse(chosen.tactic, success, round, npc);
        std::wcout << npc.name << L": " << npcResp << L"\n";

        if (success) {
            int priceDrop = 50 + chosen.successModifier * 5;
            currentPrice = std::max(currentPrice - priceDrop, medal.minPrice);
            player.reputation += 1 + (chosen.tactic == BargainTactic::FLATTERY ? 1 : 0);
            std::wcout << L"��� ������. ���� ����� �� " << priceDrop << L"\n";

            if (medal.effect == MedalEffect::MoneyBonus) {
                player.money += medal.effectValue;
                std::wcout << L"����� �� ������: +" << medal.effectValue << L"\n";
            }
        }
        else {
            player.reputation -= 1;
            int priceRise = 20;
            currentPrice += priceRise;
            std::wcout << L"������. ��������� -1. ���� ������� �� " << priceRise << L"\n";
        }

        // �������������� �������
        if (currentPrice <= medal.minPrice) {
            std::wcout << L"\n���� �������� ��������. NPC ����������� ������ ������ �� " << currentPrice << L"\n";
            player.money += currentPrice;
            player.inventory.RemoveByIndex(playerMedalIndex);
            npc.AddMedal(medal);
            std::wcout << L"������ �������. ��� ������: " << player.money << L"\n";
            return;
        }

        // ����������� ������� ������
        std::wcout << L"\n������ ������� ������ �� " << currentPrice << L"? (1=�� / ����� ������=����������)\n";
        int sellNow = ReadInt();
        if (sellNow == 1) {
            player.money += currentPrice;
            player.inventory.RemoveByIndex(playerMedalIndex);
            npc.AddMedal(medal);
            std::wcout << L"������ �������. ��� ������: " << player.money << L"\n";
            return;
        }

        ++round;
    }

    std::wcout << L"\n���� ����������. ������ �������� � ���.\n";
}