#include "MedalManager.h"

MedalManager::MedalManager() = default;

int MedalManager::Random(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

float MedalManager::GetEffectMultiplier(const std::wstring& effectOnPlayer, int playerReputation) {
    float multiplier = 1.0f;

    if (effectOnPlayer.find(L"+10% к торгу") != std::wstring::npos) {
        multiplier += 0.10f;
    }

    if (effectOnPlayer.find(L"разблокировать квест") != std::wstring::npos) {
        // Handle quest unlocking separately
    }

    multiplier += playerReputation * 0.005f;
    return multiplier;
}

int MedalManager::GetMarketValue(const Medal& medal, int playerReputation, bool buyerIsExpert) {
    if (medal.isFake && buyerIsExpert) {
        return 0;
    }

    int cachedPrice = GetCachedPrice(medal.name);
    if (cachedPrice != -1) {
        return cachedPrice;
    }

    int basePrice = Random(medal.minPrice, medal.maxPrice);

    if (medal.isFake) {
        basePrice = Random(medal.minPrice / 4, medal.maxPrice / 2);
    }

    float multiplier = GetEffectMultiplier(medal.effectOnPlayer, playerReputation);
    int finalPrice = static_cast<int>(basePrice * multiplier);

    CachePrice(medal.name, finalPrice);
    return finalPrice;
}

void MedalManager::CachePrice(const std::wstring& medalName, int price) {
    estimatedPrices[medalName] = price;
}

int MedalManager::GetCachedPrice(const std::wstring& medalName) const {
    auto it = estimatedPrices.find(medalName);
    if (it != estimatedPrices.end()) {
        return it->second;
    }
    return -1;
}

void MedalManager::ClearCache() {
    estimatedPrices.clear();
}