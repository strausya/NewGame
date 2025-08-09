#include "Medal.h"

Medal::Medal(
    const std::wstring& name,
    const std::wstring& condition,
    int minPrice,
    int maxPrice,
    MedalTier tier,
    bool isFake,
    const std::wstring& effectOnPlayer,
    int baseCost
) : name(name), condition(condition), minPrice(minPrice), maxPrice(maxPrice),
tier(tier), isFake(isFake), effectOnPlayer(effectOnPlayer), baseCost(baseCost) {
}

int Medal::GetMarketPrice() const {
    float modifier = 1.0f;
    if (condition == L"битая") modifier -= 0.3f;
    if (condition == L"поношена") modifier -= 0.1f;
    if (isFake) modifier -= 0.5f;
    return static_cast<int>(baseCost * modifier);
}