#pragma once
#include <string>
#include <vector>

enum class MedalTier {
    Budget,
    Medium,
    Valuable,
    Unique
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

    Medal(
        const std::wstring& name,
        const std::wstring& condition,
        int minPrice,
        int maxPrice,
        MedalTier tier,
        bool isFake,
        const std::wstring& effectOnPlayer,
        int baseCost = 100
    );
    int GetMarketPrice() const;
};