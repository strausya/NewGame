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
        L"спокойное", L"тревожное", L"радостное", L"унылое",
        L"бодрое", L"сонное", L"напряженное", L"праздничное"
    };

    static std::vector<std::wstring> atmospheres = {
        L"Тишину нарушает только шелест листьев",
        L"Воздух наполнен ожиданием чего-то",
        L"Чувствуется энергия большого города",
        L"Все вокруг кажется немного размытым",
        L"Ощущение, что время замедлилось",
        L"Кажется, что сегодня может случиться что-то важное"
    };

    currentWeather = static_cast<WeatherType>(std::rand() % 7);
    mood = moods[std::rand() % moods.size()];
    atmosphere = atmospheres[std::rand() % atmospheres.size()];
}

std::wstring Weather::GetWeatherDescription() const {
    switch (currentWeather) {
    case WeatherType::Sunny:    return L"Ярко светит солнце";
    case WeatherType::Cloudy:   return L"Небо затянуто облаками";
    case WeatherType::Rainy:    return L"Идет дождь";
    case WeatherType::Foggy:    return L"Все окутано туманом";
    case WeatherType::Windy:    return L"Сильный ветер";
    case WeatherType::Stormy:   return L"Гроза и ливень";
    case WeatherType::Snowy:    return L"Падает снег";
    default:                    return L"Погода неопределенная";
    }
}

std::wstring Weather::GetFullDayDescription() const {
    std::wstring description = GetWeatherDescription();
    description += L". ";
    description += atmosphere;
    description += L". ";
    description += L"Настроение ";
    description += mood;
    description += L".\n";
    return description;
}