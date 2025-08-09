#include "Weather.h"
#include "ConsoleColors.h"
#include <ctime>
#include <vector>
#include <algorithm>

Weather::Weather() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    GenerateNewWeather();
}

void Weather::GenerateNewWeather() {
    static std::vector<std::wstring> moods = {
        L"���������", L"���������", L"���������", L"������",
        L"������", L"������", L"�����������", L"�����������"
    };

    static std::vector<std::wstring> atmospheres = {
        L"������ �������� ������ ������ �������",
        L"������ �������� ��������� ����-��",
        L"����������� ������� �������� ������",
        L"��� ������ ������� ������� ��������",
        L"��������, ��� ����� �����������",
        L"�������, ��� ������� ����� ��������� ���-�� ������"
    };

    currentWeather = static_cast<WeatherType>(std::rand() % 7);
    mood = moods[std::rand() % moods.size()];
    atmosphere = atmospheres[std::rand() % atmospheres.size()];
}

std::wstring Weather::GetWeatherDescription() const {
    switch (currentWeather) {
    case WeatherType::Sunny:    return L"���� ������ ������";
    case WeatherType::Cloudy:   return L"���� �������� ��������";
    case WeatherType::Rainy:    return L"���� �����";
    case WeatherType::Foggy:    return L"��� ������� �������";
    case WeatherType::Windy:    return L"������� �����";
    case WeatherType::Stormy:   return L"����� � ������";
    case WeatherType::Snowy:    return L"������ ����";
    default:                    return L"������ ��������������";
    }
}

std::wstring Weather::GetFullDayDescription() const {
    std::wstring description = GetWeatherDescription();
    description += L". ";
    description += atmosphere;
    description += L". ";
    description += L"���������� ";
    description += mood;
    description += L".\n";
    return description;
}