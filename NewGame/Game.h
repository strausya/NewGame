#pragma once
#include "Player.h"
#include "Location.h"
#include "Inventory.h"
#include "MedalDatabase.h"
#include "GameEvents.h"
#include "SaveSystem.h"
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
    static void PrintAnimated(const std::wstring& text, int delayMs = 1);
    std::vector<std::wstring> prevActions; // предыдущее меню действий
    bool skipUI = false;
    int rentDue = 55000;
    int daysUntilEviction = 1;
    double rentIncreaseRate = 0.01;
    int utilitiesCost = 5000;
    int taxesCost = 2000;
    bool rentPaid = false;
    static const int MAX_DAY = 10; // ограничение на демонстрацию
    void CheckFinalConditions();

    struct Tax {
    std::wstring name;
    int amount;
};

std::vector<Tax> taxesList;
void InitTaxes();
void ProcessDailyPayments();


public:
    Game();
    void StartGame();
    void NextDay();
    void HandlePlayerChoice(int choice);
    void RenderUI();
    bool IsGameOver() const { return gameOver; }
    void ChangeLocation();
    void ShowLocations() const;
    void HandleEvent();
    void ShowEventMenu();
    void ShowNPCs() const;
    void InteractWithNPC();

    Player& GetPlayer() { return player; }
    const Player& GetPlayer() const { return player; }

    int GetDay() const { return day; }
    void SetDay(int d) { day = d; }

};