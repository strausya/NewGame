#pragma once
#include <string>
#include <vector>

enum class MedalTier {
    Budget,
    Medium,
    Valuable,
    Unique
};

enum class MedalEffect {
    None,
    ReputationBoost,
    HungerReduction,
    FatigueRecovery,
    BargainBonus,
    MoneyBonus,
    UnlockQuest,
    LocationInterest,
    AvoidPolice,
    RespectGroup,
    StatChange
};

struct Medal {
    std::wstring name;
    std::wstring condition;
    int minPrice;
    int maxPrice;
    MedalTier tier;
    bool isFake = false;
    std::wstring effectOnPlayer;
    int baseCost = 100;
    MedalEffect effect = MedalEffect::None;
    int effectValue = 0;

    Medal(
        const std::wstring& name,
        const std::wstring& condition,
        int minPrice,
        int maxPrice,
        MedalTier tier,
        bool isFake,
        const std::wstring& effectOnPlayer,
        int baseCost = 100,
        MedalEffect effect = MedalEffect::None,
        int effectValue = 0
    );

    int GetMarketPrice() const;
};