#include "Location.h"
#include "MedalDatabase.h"
#include <random>
#include <ctime>
#include <cstdlib>
#include <map>


void Location::AddNPC(const NPC& npc) {
    npcs.push_back(npc);
}

Location::Location(LocationType locType) : type(locType) {
    switch (locType) {
    case LocationType::BaldRock:
        name = L"����� ������";
        tradeDescription = L"������� �������� ������, �� ������ ����";
        priceModifier = 0.7f;
        fakeDetectionChance = 0.1f;
        bargainBonus = 0.0f;
        npcs.emplace_back(L"������� ��������", L"������ �������, ��������������� ������ � 1970-�", 0.2f, 0.8f, 5000);
        for (auto& npc : npcs) {
            auto randomMedals = MedalDatabase::GetRandomMedals(2 + rand() % 3);
            for (const auto& medal : randomMedals) {
                // � ��������� ������������ ������ ����� ���� ������
                if (rand() % 100 < 30) {
                    Medal fakeMedal = medal;
                    fakeMedal.isFake = true;
                    npc.AddMedal(fakeMedal);
                }
                else {
                    npc.AddMedal(medal);
                }
            }
        }
        break;

    case LocationType::GoldenSaffron:
        name = L"������� ������";
        tradeDescription = L"������� ����������, �� ����� ��������� �����������";
        priceModifier = 1.0f;
        fakeDetectionChance = 0.5f;
        bargainBonus = 0.15f;
        npcs.emplace_back(L"���� ���������", L"������ ��������, ����������� � �������", 0.5f, 0.3f, 20000);
        for (auto& npc : npcs) {
            auto randomMedals = MedalDatabase::GetRandomMedals(2 + rand() % 3);
            for (const auto& medal : randomMedals) {
                // � ��������� ������������ ������ ����� ���� ������
                if (rand() % 100 < 30) {
                    Medal fakeMedal = medal;
                    fakeMedal.isFake = true;
                    npc.AddMedal(fakeMedal);
                }
                else {
                    npc.AddMedal(medal);
                }
            }
        }
        break;

    case LocationType::MegaCollector:
        name = L"������������ � ����";
        tradeDescription = L"������� ������������ - ������ �����, �� �� ���������";
        priceModifier = 1.5f;
        fakeDetectionChance = 0.9f;
        bargainBonus = 0.0f;
        break;

    case LocationType::Stakha:
        name = L"�����";
        tradeDescription = L"����� ����� ��������� ������, �� ������ �������";
        priceModifier = 0.5f;
        fakeDetectionChance = 0.0f;
        bargainBonus = 0.3f;
        break;
    }
}

Location::Location(const std::wstring& name, const std::wstring& description) :
    name(name),
    tradeDescription(description),
    type(LocationType::BaldRock), // Default type
    priceModifier(1.0f),
    fakeDetectionChance(0.5f),
    bargainBonus(0.0f)
{
}

std::wstring Location::GetRandomEvent() const {
    static std::random_device rd;
    static std::mt19937 gen(rd());

    static std::map<LocationType, std::vector<std::wstring>> events = {{
        LocationType::BaldRock, {
            L"������� ��������� ���� ������� �� ����� ���������",
            L"����� ������ ������ � �������",
            L"���-�� ��������� ���� ���"
        }},
        {LocationType::GoldenSaffron, {
            L"� ���� �������� ���������� ���",
            L"�� �������� �������� �������������"
        }},
        {LocationType::MegaCollector, {
            L"�� �������� ������� ������",
            L"������������ � ������� ����������"
        }},
        {LocationType::Stakha, {
            L"���-�� ������� ��������� ���",
            L"������� ������������ �����"
        }}
    };
    const auto it = events.find(type);
    if (it != events.end()) {
        const auto& locEvents = it->second;
        std::uniform_int_distribution<int> dis(0, static_cast<int>(locEvents.size()) - 1);
        return locEvents[dis(gen)];
    }
    return L"������ �� ����������.";
}

std::wstring Location::GetTradeDescription() const {
    switch (type) {
    case LocationType::BaldRock:
        return L"������� �������� ������, �� ������ ����";
    case LocationType::GoldenSaffron:
        return L"������� ����������, �� ����� ��������� �����������";
    case LocationType::MegaCollector:
        return L"������� ������������ - ������ �����, �� �� ���������";
    case LocationType::Stakha:
        return L"����� ����� ��������� ������, �� ������ �������";
    default:
        return L"�������� ����� ��� ��������...";
    }
}

std::vector<GameEvent> Location::GenerateDailyEvents() const {
    std::vector<GameEvent> events;
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> eventDist(0, 100);

    // ������� ���� ������� (30%)
    if (eventDist(gen) > 70) return events;

    GameEvent event;
    std::uniform_int_distribution<> typeDist(0, 3);

    switch (typeDist(gen)) {
    case 0: // ����������� �������
        event.type = EventType::Neutral;
        event.description = GetRandomEvent();
        break;
    case 1: // �������������
        event.type = EventType::Positive;
        event.description = L"���� �������! " + GetRandomEvent();
        event.moneyEffect = 50 + (rand() % 200);
        break;
    case 2: // �������������
        event.type = EventType::Negative;
        event.description = L"�������! " + GetRandomEvent();
        event.moneyEffect = -(20 + (rand() % 100));
        break;
    case 3: // ������ (� ����������)
        event.type = EventType::Special;
        event.description = L"������ ������! " + GetRandomEvent();
        event.items = MedalDatabase::GetRandomMedals(1);
        break;
    }

    events.push_back(event);
    return events;
}



