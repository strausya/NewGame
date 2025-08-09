#pragma once
#include <string>
#include <vector>
#include "GameEvents.h"
#include "NPC.h"

enum class LocationType {
    BaldRock,
    GoldenSaffron,
    MegaCollector,
    Stakha
};

class Location {
public:
    std::wstring name;
    std::wstring tradeDescription;
    LocationType type;
    float priceModifier = 1.0f;
    float fakeDetectionChance = 0.5f;
    float bargainBonus = 0.0f;
    std::vector<NPC> npcs;
    void AddNPC(const NPC& npc);


    explicit Location(LocationType locType);
    Location(const std::wstring& name, const std::wstring& description);

    std::wstring GetTradeDescription() const;
    std::wstring GetRandomEvent() const;
    std::vector<GameEvent> GenerateDailyEvents() const;
};
