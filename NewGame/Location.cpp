#include "Location.h"
#include "MedalDatabase.h"
#include <random>
#include <ctime>
#include <cstdlib>
#include <map>

NPC::NPC(const std::wstring& name, const std::wstring& backstory, NPCType type,
    float bargainDifficulty, float gullibility, int money)
    : name(name), backstory(backstory), type(type),
    bargainDifficulty(bargainDifficulty), gullibility(gullibility), money(money) {
    InitializeDialogs();
}

void Location::AddNPC(const NPC& npc) {
    npcs.push_back(npc);
}

void Location::InitializeNPCs() {
    switch (type) {
    case LocationType::BaldRock:
        npcs.emplace_back(L"������� ��������", L"������ �������, ��������������� ������ � 1970-�", NPCType::VETERAN, 0.2f, 0.8f, 5000);
        npcs.emplace_back(L"���� ����", L"������� ������ ������ ��������� ����", NPCType::TRADER, 0.3f, 0.7f, 3000);
        npcs.emplace_back(L"������", L"�������� ���������� � �������� �����, ����� ����������� ������� ������", NPCType::TRADER, 0.4f, 0.5f, 2500);
        npcs.emplace_back(L"����� �����", L"������������ �����, �������� ��������� ������", NPCType::HOBBYIST, 0.1f, 0.6f, 500);
        break;

    case LocationType::GoldenSaffron:
        npcs.emplace_back(L"���� ���������", L"������ ��������, ����������� � �������", NPCType::COLLECTOR, 0.5f, 0.3f, 20000);
        npcs.emplace_back(L"�������", L"������� ������������, ���� ������ ����������", NPCType::HOBBYIST, 0.4f, 0.5f, 15000);
        npcs.emplace_back(L"���������", L"���������-�������, ������������ ��������� ��� ��������", NPCType::HOBBYIST, 0.3f, 0.4f, 8000);
        npcs.emplace_back(L"�����", L"��������� � �������� ������, �������� �������� ���-�� ������������", NPCType::TRADER, 0.2f, 0.6f, 1200);
        break;

    case LocationType::MegaCollector:
        npcs.emplace_back(L"������ ��������", L"������� ������������, ����� �� � �������", NPCType::COLLECTOR, 0.7f, 0.1f, 30000);
        npcs.emplace_back(L"����", L"�������� �������, ������� ��������� � �������� �����", NPCType::HOBBYIST, 0.3f, 0.4f, 5000);
        npcs.emplace_back(L"�����", L"�������������� ������� � ������� ������, ����������", NPCType::TRADER, 0.2f, 0.7f, 4000);
        break;

    case LocationType::Stakha:
        npcs.emplace_back(L"���� ����", L"������� �����������, ����� �����������", NPCType::TRADER, 0.1f, 0.9f, 1000);
        npcs.emplace_back(L"����", L"������ � ������, ������� ��� ������", NPCType::HOBBYIST, 0.2f, 0.6f, 500);
        npcs.emplace_back(L"����", L"����������� ������� � ���������� �������, ������� ������ '�� ������'", NPCType::TRADER, 0.3f, 0.5f, 1500);
        npcs.emplace_back(L"����� �����", L"������� ��������, �������� ������� ������ ������", NPCType::TRADER, 0.1f, 0.8f, 300);
        npcs.emplace_back(L"���� '��������'", L"���������, ��������� � ������, ������� ��������� �� �������", NPCType::HOBBYIST, 0.1f, 0.5f, 700);
        break;
    }

    std::random_device rd;
    std::mt19937 gen(rd());

    for (auto& npc : npcs) {
        int medalsCount = 0;

        switch (npc.type) {
        case NPCType::COLLECTOR: {
            std::uniform_int_distribution<> dist(5, 8);
            medalsCount = dist(gen);
            break;
        }
        case NPCType::VETERAN: {
            std::uniform_int_distribution<> dist(1, 2);
            medalsCount = dist(gen);
            break;
        }
        case NPCType::TRADER: {
            std::uniform_int_distribution<> dist(3, 5);
            medalsCount = dist(gen);
            break;
        }
        case NPCType::HOBBYIST: {
            std::uniform_int_distribution<> dist(1, 3);
            medalsCount = dist(gen);
            break;
        }
        }

        auto medals = MedalDatabase::GetRandomMedals(medalsCount);

        for (auto& medal : medals) {
            if (npc.type == NPCType::TRADER) {
                std::uniform_int_distribution<> fakeChance(0, 99);
                if (fakeChance(gen) < 30) {
                    medal.isFake = true;
                }
            }
            if (npc.type == NPCType::VETERAN) {
                medal.tier = MedalTier::Unique;
            }
            npc.medalsForSale.push_back(medal);
        }
    }
}

Location::Location(LocationType locType) : type(locType) {
    switch (locType) {
    case LocationType::BaldRock:
        name = L"����� ������";
        tradeDescription = L"������� �������� ������, �� ������ ����";
        priceModifier = 0.7f;
        fakeDetectionChance = 0.1f;
        bargainBonus = 0.0f;
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

    InitializeNPCs();
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

    static std::map<LocationType, std::vector<std::wstring>> events = {
        {LocationType::BaldRock, {
            L"������� ��������� ���� ������� �� ����� ���������",
            L"����� ������ ������ � �������",
            L"���-�� ��������� ���� ���",
            L"�������� ������� ���� � ���� �� ������ ������",
            L"������� ������� �������� ��� ������� �����",
            L"�������� �������� ���������� ������ ���� � ���������",
            L"���� ������ ������� � ������ ���������� ������������� ���������",
            L"�� ������ ���������� � ������� '������������' �������"
        }},
        {LocationType::GoldenSaffron, {
            L"� ���� �������� ���������� ���",
            L"�� �������� �������� �������������",
            L"������� �������, ���������������� �� �� ���-��",
            L"� ���� ����� �������, ��������������� ������ ��� �����",
            L"����� ����������� �� ���� ��� �� ������",
            L"�������� ��������, ��� ����� '����� �� ������' � ������",
            L"�������� � ���� ����� ������� ����� �������",
            L"���� ���������� �������� ����� ������ �� ����������� ��������"
        }},
        {LocationType::MegaCollector, {
            L"�� �������� ������� ������",
            L"������������ � ������� ����������",
            L"���-�� ������ �������� � ��� ������� �������",
            L"�� ������� ������, ������� �� ���� �� �����",
            L"������� �������� ������ � ���������",
            L"�������� ������� ��������� ���������� � �������� ����������",
            L"���� �� ��������� ��������� ���� '����� ����������'",
            L"������� ��������� ���� �� ������� �������� ���������"
        }},
        {LocationType::Stakha, {
            L"���-�� ������� ��������� ���",
            L"������� ������������ �����",
            L"������ ���� ��������� ���������� ������",
            L"�� ����� ����� ���������� � ��������� ������",
            L"�� ������� ��������������� �������� � ������ ��������",
            L"���� ��������� ������� ������, �� ��� ��������� ����������",
            L"������� � ������ ���������� ���� '����� �������� ������'",
            L"�� ������, ��� ���� ������ � ����������� ������"
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



