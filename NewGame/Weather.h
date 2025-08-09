#pragma once
#include "ConsoleColors.h"
#include <string>
#include <vector>

enum class WeatherType {
    Sunny,
    Cloudy,
    Rainy,
    Foggy,
    Windy,
    Stormy,
    Snowy
};

class Weather {
private:
    WeatherType currentWeather;
    std::wstring mood;
    std::wstring atmosphere;

public:
    Weather();
    void GenerateNewWeather();
    std::wstring GetWeatherDescription() const;
    std::wstring GetFullDayDescription() const;
    WeatherType GetCurrentWeather() const {
        return currentWeather;
    }
};

