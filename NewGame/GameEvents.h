#pragma once
#include <vector>
#include <string>
#include "ConsoleColors.h"
#include "Medal.h"

enum class EventType {
    Neutral,
    Positive,
    Negative,
    Special
};

struct GameEvent {
    std::wstring description;
    EventType type;
    int moneyEffect = 0;
    int reputationEffect = 0;
    std::vector<Medal> items;
};