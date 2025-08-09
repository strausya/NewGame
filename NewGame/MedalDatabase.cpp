#include "MedalDatabase.h"
#include "Medal.h"

static const std::vector<Medal> medals = {
    {L"������ ��� ������", L"�������", 3000, 20000, MedalTier::Valuable, false, L"+���������: �������� ���� �����", 100},
    {L"������ �������� �����", L"��������", 200, 500, MedalTier::Budget, false, L"-20% � ������ �����", 100},
    {L"������ �50 ��� ������ � ��»", L"�����", 100, 300, MedalTier::Budget, false, L"+��������: ����� �������� �������", 100},
    {L"������������ ������", L"�������", 15000, 90000, MedalTier::Valuable, false, L"+10% � �����", 100},
    {L"����� ������� ������� IV �������", L"��������", 250000, 1200000, MedalTier::Unique, false, L"��������������_�����:�������������_����", 100},
    {L"������ �� ������ ����� 1812 ����", L"������������������", 20000, 70000, MedalTier::Valuable, false, L"���������� ���� �������������", 100},
    {L"������ ��� �������� ����� 1877�1878 ��.�", L"������������������", 10000, 30000, MedalTier::Valuable, false, L"������� � �������� � ���������� �������", 100},
    {L"����� ������ ���� III �������", L"�������", 30000, 150000, MedalTier::Valuable, true, L"��� ������ � ���� ��� �� Avito", 100},
    {L"������ ���������� ���������� ������� II�", L"��������", 15000, 40000, MedalTier::Valuable, true, L"�������� ������������� ������������� ��������", 100},
    {L"����� ��������� ��������� �����������", L"�������", 1000, 5000, MedalTier::Budget, false, L"������ �� ��, ��� �� ������� � XIX ����. � �� �������� � ��������", 100},
    {L"��������� ����� ��� �������", L"�������", 5000, 20000, MedalTier::Medium, false, L"����� ������", 100},
    {L"������ ��� ������ �������", L"�������", 2000, 10000, MedalTier::Medium, false, L"+10% � ���� � �����", 100},
    {L"������ ��� ������������ �����", L"������������������", 800, 2500, MedalTier::Budget, false, L"���� ��� �������� ������", 100},
    {L"������ �60 ��� ���������� ��� ���л", L"�����", 100, 300, MedalTier::Budget, false, L"+15% ���� ��������� ������ ����", 100},
    {L"������ ��� �������� �������", L"�������", 1500, 6000, MedalTier::Medium, false, L"��������� ����� ���������", 100},
    {L"������ ��� ���������� ����", L"��������", 1000, 2000, MedalTier::Budget, false, L"������� ������ '���� �� �����'", 100},
    {L"������ ��� ������������� ����", L"�������", 1500, 3500, MedalTier::Medium, false, L"", 100},
    {L"������ ��� ���������� ������� �����������", L"�����", 1000, 2500, MedalTier::Medium, false, L"������������ VIP-������� � ������", 100},
    {L"������ ��� �������� �������", L"������������������", 500, 1500, MedalTier::Budget, false, L"������ �������� ���������", 100},
    {L"������ ��� ������ ��� ��ջ", L"�����������", 0, 0, MedalTier::Budget, true, L"����� ����� � ����������� ����������. ����� ������ �������", 100},
    {L"������ ��� �������� ����� ��ݻ", L"���������", 100, 300, MedalTier::Budget, true, L"+5 � ����������", 100},
    {L"������ ������� ��� ����", L"�����", 150, 350, MedalTier::Budget, false, L"-10 � ��������", 100},
    {L"����� ������� ��������� IV �������", L"��������", 40000, 180000, MedalTier::Valuable, true, L"�������� ��������", 100},
    {L"����� ������� ���������� III �������", L"�������", 20000, 80000, MedalTier::Medium, true, L"����� �������� ��� �����", 100},
    {L"����� ������ ��������� (����� ��������� ����)", L"���������", 2000000, 5000000, MedalTier::Unique, false, L"������ �� ������", 100}
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

std::vector<Medal> MedalDatabase::GetRandomMedals(int count) {
    std::vector<Medal> result;
    const auto& allMedals = GetAllMedals();

    if (allMedals.empty()) return result;

    // ������� ����� ��� �������������
    std::vector<Medal> shuffled = allMedals;

    // ������������ ������
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(shuffled.begin(), shuffled.end(), g);

    // ����� ������ 'count' �������
    int take = std::min(count, (int)shuffled.size());
    result.assign(shuffled.begin(), shuffled.begin() + take);

    return result;
}
