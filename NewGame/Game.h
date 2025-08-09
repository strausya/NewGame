#pragma once
#include "Player.h"
#include "Location.h"
#include "Inventory.h"
#include "MedalDatabase.h"
#include "GameEvents.h"
#include "Weather.h"
#include <thread>
#include <chrono>
#include <vector>

extern std::vector<Location> availableLocations;

class Game {
private:
    Player player;
    Weather weather;
    Location currentLocation;
    static std::vector<Location> availableLocations;
    int day = 1;
    bool gameOver = false;
    bool waitingForDayEnd = false;
    std::vector<GameEvent> currentEvents;
    bool eventActive = false;
    void ApplyWeatherEffects();
    int actionsToday = 0;        // —четчик действий за день
    bool isOverworked = false;   // ‘лаг перегруза
    bool firstActionOfDay;
    static void PrintAnimated(const std::wstring& text, int delayMs = 30);

public:
    Game();
    void StartGame();
    void NextDay();
    void HandlePlayerChoice(int choice);
    void RenderUI();
    bool IsGameOver() const { return gameOver; }
    void ChangeLocation();
    void ShowLocations() const;
    //void ConfirmNextDay();
    void HandleEvent();
    void ShowEventMenu();
    void ShowNPCs() const;
    void InteractWithNPC();

private:
    void CheckFinalConditions();
};