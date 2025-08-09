#pragma once
#include <map>
#include <random>
#include "Medal.h"

class MedalManager {
public:
    MedalManager();

    int GetMarketValue(const Medal& medal, int playerReputation = 0, bool buyerIsExpert = false);
    void CachePrice(const std::wstring& medalName, int price);
    int GetCachedPrice(const std::wstring& medalName) const;
    void ClearCache();

private:
    std::map<std::wstring, int> estimatedPrices;

    int Random(int min, int max);
    float GetEffectMultiplier(const std::wstring& effectOnPlayer, int playerReputation);
};