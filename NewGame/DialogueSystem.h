#pragma once
#include <string>
#include <vector>
#include "Player.h"
#include "NPC.h"
#include "Medal.h"

struct DialogueOption {
    std::wstring text;
    BargainTactic tactic;
    int successModifier;
    int unlockReputation;
};

class DialogueSystem {
public:
    static std::wstring GetNPCResponse(BargainTactic tactic, bool success, int round, const NPC& npc);
    static int CalculateChance(const DialogueOption& opt, const Player& player, const NPC& npc, const Medal& medal, int round);
    static void StartMultiRoundDialogue(Player& player, NPC& npc, size_t playerMedalIndex, int startingPrice);

private:
    static int ReadInt();
    static bool CheckSuccess(int chance);
};