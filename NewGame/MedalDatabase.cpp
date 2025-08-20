#include "MedalDatabase.h"
#include "Medal.h"

static const std::vector<Medal> medals = {
    {L"������ ��� ������", L"�������", 3000, 20000, MedalTier::Valuable, false,
     L"+���������: �������� ���� �����",
     100, MedalEffect::ReputationBoost, 3},

    {L"������ �������� �����", L"��������", 200, 500, MedalTier::Budget, false,
     L"-20% � ������ �����",
     100, MedalEffect::ReputationBoost, -2},

    {L"������ �50 ��� ������ � ��»", L"�����", 100, 300, MedalTier::Budget, false,
     L"+��������: ����� �������� �������",
     100, MedalEffect::MoneyBonus, 200},

    {L"������������ ������", L"�������", 15000, 90000, MedalTier::Valuable, false,
     L"+10% � �����",
     100, MedalEffect::BargainBonus, 10},

    {L"����� ������� ������� IV �������", L"��������", 250000, 1200000, MedalTier::Unique, false,
     L"������������ �����: �������������� ����",
     100, MedalEffect::ReputationBoost, 10},

    {L"������ �� ������ ����� 1812 ����", L"������������������", 20000, 70000, MedalTier::Valuable, false,
     L"���������� ���� �������������",
     100, MedalEffect::None, 0},

    {L"������ ��� �������� ����� 1877�1878 ��.�", L"������������������", 10000, 30000, MedalTier::Valuable, false,
     L"������� � ���������� ��������",
     100, MedalEffect::ReputationBoost, 2},

    {L"����� ������ ���� III �������", L"�������", 30000, 150000, MedalTier::Valuable, true,
     L"��� ������ � ���� ��� �� Avito",
     100, MedalEffect::None, 0},

    {L"������ ���������� ���������� ������� II�", L"��������", 15000, 40000, MedalTier::Valuable, true,
     L"�������� �������������-����������",
     100, MedalEffect::MoneyBonus, 500},

    {L"����� ��������� ��������� �����������", L"�������", 1000, 5000, MedalTier::Budget, false,
     L"������ �� ��, ��� �� ������� � XIX ����. � �� �������� � ��������",
     100, MedalEffect::None, 0},

    {L"��������� ����� ��� �������", L"�������", 5000, 20000, MedalTier::Medium, false,
     L"����� ������",
     100, MedalEffect::FatigueRecovery, 3},

    {L"������ ��� ������ �������", L"�������", 2000, 10000, MedalTier::Medium, false,
     L"+10% � ���� � �����",
     100, MedalEffect::MoneyBonus, 1000},

    {L"������ ��� ������������ �����", L"������������������", 800, 2500, MedalTier::Budget, false,
     L"���� ��� �������� ������",
     100, MedalEffect::None, 0},

    {L"������ �60 ��� ���������� ��� ���л", L"�����", 100, 300, MedalTier::Budget, false,
     L"+15% ���� ��������� ������ ����",
     100, MedalEffect::BargainBonus, 5},

    {L"������ ��� �������� �������", L"�������", 1500, 6000, MedalTier::Medium, false,
     L"��������� ����� ���������",
     100, MedalEffect::FatigueRecovery, 2},

    {L"������ ��� ���������� ����", L"��������", 1000, 2000, MedalTier::Budget, false,
     L"������� ������ '���� �� �����'",
     100, MedalEffect::ReputationBoost, 1},

    {L"������ ��� ������������� ����", L"�������", 1500, 3500, MedalTier::Medium, false,
     L"�",
     100, MedalEffect::None, 0},

    {L"������ ��� ���������� ������� �����������", L"�����", 1000, 2500, MedalTier::Medium, false,
     L"������������ VIP-������� � ������",
     100, MedalEffect::ReputationBoost, 4},

    {L"������ ��� �������� �������", L"������������������", 500, 1500, MedalTier::Budget, false,
     L"������ �������� ���������",
     100, MedalEffect::HungerReduction, 1},

    {L"������ ��� ������ ��� ��ջ", L"�����������", 0, 0, MedalTier::Budget, true,
     L"����� ����� � ����������� ����������. ����� ������ �������",
     100, MedalEffect::FatigueRecovery, 5},

    {L"������ ��� �������� ����� ��ݻ", L"���������", 100, 300, MedalTier::Budget, true,
     L"+5 � ����������",
     100, MedalEffect::ReputationBoost, 2},

    {L"������ ������� ��� ����", L"�����", 150, 350, MedalTier::Budget, false,
     L"-10 � ��������",
     100, MedalEffect::None, 0},

    {L"����� ������� ��������� IV �������", L"��������", 40000, 180000, MedalTier::Valuable, true,
     L"�������� ��������",
     100, MedalEffect::ReputationBoost, 6},

    {L"����� ������� ���������� III �������", L"�������", 20000, 80000, MedalTier::Medium, true,
     L"����� �������� ��� �����",
     100, MedalEffect::MoneyBonus, 2000},

    {L"����� ������ ��������� (����� ��������� ����)", L"���������", 2000000, 5000000, MedalTier::Unique, false,
     L"������ �� ������",
     100, MedalEffect::ReputationBoost, 15}
};

const std::vector<Medal>& MedalDatabase::GetAllMedals() {
    return medals;
}

const Medal* MedalDatabase::FindByName(const std::wstring& name) {
    for (const auto& medal : medals) {
        if (medal.name == name)
            return &medal;
    }
    return nullptr;
}

std::vector<Medal> MedalDatabase::GetCheapestMedals(int count) {
    std::vector<Medal> sorted = medals;
    std::sort(sorted.begin(), sorted.end(), [](const Medal& a, const Medal& b) {
        return a.minPrice < b.minPrice;
        });
    if (count > (int)sorted.size()) count = (int)sorted.size();
    return std::vector<Medal>(sorted.begin(), sorted.begin() + count);
}

std::vector<Medal> MedalDatabase::GetRandomMedals(int count) {
    const auto& all = GetAllMedals();
    std::vector<Medal> result;
    if (all.empty()) return result;

    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_int_distribution<int> dist(0, (int)medals.size() - 1);

    for (int i = 0; i < count; ++i) {
        result.push_back(all[dist(g)]);
    }
    return result;
}