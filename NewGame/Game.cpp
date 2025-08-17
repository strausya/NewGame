#include "Game.h"
#include <iostream>
#include "ConsoleColors.h"
#include <limits>
#include <string>
#include <Windows.h>


std::vector<Location> Game::availableLocations = {
    Location(LocationType::BaldRock),
    Location(LocationType::GoldenSaffron),
    Location(LocationType::MegaCollector),
    Location(LocationType::Stakha)
};

Game::Game() : currentLocation(LocationType::Stakha) {
    firstActionOfDay = true;
    InitTaxes();

}

void Game::InitTaxes() {
    taxesList = {
        {L"��������� ����� �� �������� ��������������� ������������", 1400},
        {L"����� �� �������������� ������������ �������", 2200},
        {L"���� �� ��������� ���������������� ��������", 900},
        {L"����� �� �������� ������������� ���������", 1750},
        {L"����� �� ��������� ������������ ���������", 1300},
        {L"����� �� ���������� � ������������ ��������� �������", 800},
        {L"����� �� ��������������� � ���������� ��������� �����", 1950},
        {L"����� �� �������� �������������� ����������", 1650},
        {L"���� �� �������� ������ ���� ������ ��� ����", 2100},
        {L"������������ ����� � ���� ������������ ���", 2500}
    };
}

void Game::PrintAnimated(const std::wstring& text, int delayMs) {
    for (wchar_t c : text) {
        std::wcout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
    }
}

void Game::StartGame() {

    std::wcout << L"�����������������������������������������������������������������������������������������������������" << std::endl;
    std::wcout << L": ________  ___  ___  ___  ________  _________  ___  ___       ________  ___  ___  ___  _________   :" << std::endl;
    std::wcout << L":|\\   ____\\|\\  \\|\\  \\|\\  \\|\\   ____\\|\\___   ___\\\\  \\|\\  \\     |\\   ____\\|\\  \\|\\  \\|\\  \\|\\___   ___\\ :" << std::endl;
    std::wcout << L":\\ \\  \\___|\\ \\  \\\\\\  \\ \\  \\ \\  \\___|\\|___ \\  \\_\\ \\  \\ \\  \\    \\ \\  \\___|\\ \\  \\\\\\  \\ \\  \\|___ \\  \\_| :" << std::endl;
    std::wcout << L": \\ \\  \\    \\ \\   __  \\ \\  \\ \\_____  \\   \\ \\  \\ \\ \\  \\ \\  \\    \\ \\_____  \\ \\   __  \\ \\  \\   \\ \\  \\  :" << std::endl;
    std::wcout << L":  \\ \\  \\____\\ \\  \\ \\  \\ \\  \\|____|\\  \\   \\ \\  \\ \\ \\  \\ \\  \\____\\|____|\\  \\ \\  \\ \\  \\ \\  \\   \\ \\  \\ :" << std::endl;
    std::wcout << L":   \\ \\_______\\ \\__\\ \\__\\ \\__\\____\\_\\  \\   \\ \\__\\ \\ \\__\\ \\_______\\____\\_\\  \\ \\__\\ \\__\\ \\__\\   \\ \\__\\:" << std::endl;
    std::wcout << L":    \\|_______|\\|__|\\|__|\\|__|\\_________\\   \\|__|  \\|__|\\|_______|\\_________\\|__|\\|__|\\|__|    \\|__|:" << std::endl;
    std::wcout << L":                            \\|_________|                        \\|_________|                       :" << std::endl;
    std::wcout << L"����������������������������� ���������� ���������귷��������������������������������������������������������������" << std::endl;





    Beep(392, 300);  // ���� (G4)
    Beep(440, 300);  // �� (A4)
    Beep(392, 300);  // ���� (G4)
    Beep(330, 600);  // �� (E4) � ������� ����

    auto startingMedals = MedalDatabase::GetCheapestMedals(10);
    for (const auto& medal : startingMedals) {
        player.AddMedal(medal);
    }

    for (const auto& medal : startingMedals) {
        player.AddMedal(medal);
      }

    PrintAnimated(L"�� � ������. � ���� ��� �����. ������ ������ � ��������.\n");
    std::wcin.ignore((std::numeric_limits<std::streamsize>::max)(), L'\n');
    PrintAnimated(L"����� Enter, �������, ���������. ������� ���������: \n !!!!!!������ ����!!!!!\n");
    std::wcin.ignore((std::numeric_limits<std::streamsize>::max)(), L'\n');
}

void Game::ChangeLocation() {
    ShowLocations();
    std::wcout << L"���� �������? (1-" << availableLocations.size() << "): ";

    int choice;
    std::wcin >> choice;

    if (choice >= 1 && choice <= static_cast<int>(availableLocations.size())) {
        currentLocation = availableLocations[choice - 1];
        PrintAnimated(L"�� ���������� � " + currentLocation.name + L"\n");
        PrintAnimated(currentLocation.GetTradeDescription() + L"\n");
    }
    else {
        std::wcout << L"�������� �����! ��������� � ������� �������.\n";
    }
}

void Game::ShowLocations() const {
    std::wcout << L"\n=== ��������� ������� ===\n";
    for (size_t i = 0; i < availableLocations.size(); ++i) {
        std::wcout << i + 1 << L". " << availableLocations[i].name << L"\n";
    }
}

void Game::RenderUI() {

    if (skipUI) {
        skipUI = false; // �����
        return;
    }
    ConsoleColors::SetColor(ConsoleColors::YELLOW);
    std::wcout << L"\n...........................................\n";

    ConsoleColors::SetColor(ConsoleColors::CYAN);
    std::wcout << L"=== ���� " << day << L" ===\n";
    std::wcout << weather.GetWeatherDescription() << L"\n";
    ConsoleColors::Reset();
    //player.ShowChangedStats();
    player.ShowStats();

    ConsoleColors::SetColor(ConsoleColors::GREEN);
    PrintAnimated(L"�������: " + currentLocation.name + L"\n");

    ConsoleColors::Reset();
        std::vector<std::wstring> actions = { L"������ ���� � ���� \n2. ������� � ���������. \n3. ��������� \n 4. �������� \n 5. ������� ������� \n6. ���������� � NPC\n7. ��������� ����\n ��� ���������?: " };
        std::wcout << L"\n=== ��������� �������� ===\n";
        for (size_t i = 0; i < actions.size(); ++i) {
            std::wcout << i + 1 << L". " << actions[i] << L"\n";
        }

    if (actionsToday > 9) {
        ConsoleColors::SetColor(ConsoleColors::RED);
        std::wcout << L"��������! ��������� �������� ������� ��������!\n";
        ConsoleColors::Reset();
    }

}

void Game::HandlePlayerChoice(int choice) {

    if (eventActive) {
        HandleEvent();
        return;
    }
 if (choice >= 1 && choice <= 5) {
     actionsToday++;

            // �������� �� ��������
            if (actionsToday > 10) {
                player.fatigue += 10;
                isOverworked = true;

                ConsoleColors::SetColor(ConsoleColors::RED);
                std::wcout << L"������� ����� ��������! ��������� +10\n";
                ConsoleColors::Reset();
            }
        }
 
    switch (choice) {
    case 1: player.EatFood(); skipUI = true; break;
    case 2: player.Rest(); skipUI = true; break;
    case 3: player.Trade(currentLocation);  break;
    case 4:
        player.BuyFromNPC(currentLocation);
        break;
    case 5: ChangeLocation(); skipUI = false; break;
    case 6: InteractWithNPC(); skipUI = true; break;
    case 7:
        NextDay();
        return; 

    default: std::wcout << L"�������� �����!\n";
    }

    if (!eventActive && actionsToday >= 5) {
        NextDay();
    }
}

void Game::ApplyWeatherEffects() {
    WeatherType currentWeather = weather.GetCurrentWeather();

    if (currentWeather == WeatherType::Rainy || currentWeather == WeatherType::Stormy) {
        player.fatigue += 20;
    }
    else if (currentWeather == WeatherType::Sunny) {
        player.hunger += 5;
    }
    else if (currentWeather == WeatherType::Snowy) {
        player.hunger += 10;
        player.fatigue += 10;
    }
}

void Game::ProcessDailyPayments() {
    std::random_device rd;
    std::mt19937 rng(rd());

    // ��������� ���
    player.money -= utilitiesCost;
    std::wcout << L"� ��� ����� " << utilitiesCost << L" ���. �� ���.\n";

    // �������� ��������� �����
    if (!taxesList.empty()) {
        std::uniform_int_distribution<int> dist(0, static_cast<int>(taxesList.size()) - 1);
        const Tax& selectedTax = taxesList[dist(rng)];

        // ������������� ����� ������
        std::uniform_real_distribution<> variation(0.8, 1.2);
        int finalAmount = static_cast<int>(selectedTax.amount * variation(rng));

        player.money -= finalAmount;
        std::wcout << L"� ��� ����� " << finalAmount << L" ���. � " << selectedTax.name << L".\n";
    }

    // ������ ��������
    if (!rentPaid) {
        if (daysUntilEviction > 0) {
            std::wcout << L"� ��� ���� " << daysUntilEviction
                << L" ����, ����� ��������� �� ��������: " << rentDue << L" ���.\n";
            daysUntilEviction--;
        }
        else {
            rentDue = static_cast<int>(rentDue * (1.0 + rentIncreaseRate));
            std::wcout << L"�� �� ��������� �������! ���� ����� �� " << rentDue << L" ���.\n";
        }
    }
}

void Game::NextDay() {
    weather.GenerateNewWeather();


    if (isOverworked) {
        player.fatigue += 30;
        player.reputation -= 5;
        ConsoleColors::SetColor(ConsoleColors::RED);
        std::wcout << L"�� ����������� �����! ��������� +30, ��������� -5\n";
        ConsoleColors::Reset();
        isOverworked = false;
    }

    actionsToday = 0;
    firstActionOfDay = true;
    ++day;
    if (day > MAX_DAY) {
        gameOver = true;
        std::wcout << L"\n������������ ���������. �������� �����...\n";
        CheckFinalConditions();
        player.ShowStats();
        return;
    }

    actionsToday = 0;
    firstActionOfDay = true;
    rentPaid = false;
    player.hunger += 10;
    player.fatigue += 12; 
    
    ConsoleColors::SetColor(ConsoleColors::YELLOW);
    std::wcout << L"\n=== ���� " << day << L" ===\n";
    ConsoleColors::Reset();

    switch (weather.GetCurrentWeather()) {
    case WeatherType::Sunny:
        ConsoleColors::SetColor(ConsoleColors::YELLOW);
        break;
    case WeatherType::Rainy:
    case WeatherType::Stormy:
        ConsoleColors::SetColor(ConsoleColors::BLUE);
        break;
    case WeatherType::Snowy:
        ConsoleColors::SetColor(ConsoleColors::WHITE);
        break;
    default:
        ConsoleColors::SetColor(ConsoleColors::CYAN);
    }
    std::wcout << weather.GetFullDayDescription();
    ConsoleColors::Reset();
   
    player.ShowStats();
    prevActions.clear();

    CheckFinalConditions();

    ApplyWeatherEffects();

    currentEvents = currentLocation.GenerateDailyEvents();
    if (!currentEvents.empty()) {
        eventActive = true;
    }

    ProcessDailyPayments();

}

void Game::ShowEventMenu() {
    ConsoleColors::SetColor(ConsoleColors::MAGENTA);
    PrintAnimated(L"��������� ���!!!");
    ConsoleColors::Reset();

    for (const auto& event : currentEvents) {
        // ����� �������� ������� � ������ �� ����
        switch (event.type) {
        case EventType::Positive:
            ConsoleColors::SetColor(ConsoleColors::GREEN);
            break;
        case EventType::Negative:
            ConsoleColors::SetColor(ConsoleColors::RED);
            break;
        default:
            ConsoleColors::SetColor(ConsoleColors::YELLOW);
        }

        std::wcout << event.description << L"\n";
        ConsoleColors::Reset();
    }

    std::wcout << L"\n1. ����������\n";
    if (currentEvents[0].type == EventType::Special) {
        std::wcout << L"2. ����� �������\n";
    }
}

void Game::HandleEvent() {
    int choice;
    std::wcin >> choice;

    auto& event = currentEvents[0];
    switch (choice) {
    case 1: // ������ ����������
        break;
    case 2: // ��� ����������� �������
        if (event.type == EventType::Special && !event.items.empty()) {
            player.AddMedal(event.items[0]);
            std::wcout << L"�������� ������: " << event.items[0].name << L"\n";
        }
        break;
    }

    // ��������� �������
    player.money += event.moneyEffect;
    player.reputation += event.reputationEffect;

    // ��������� ��������� �������
    eventActive = false;
    currentEvents.clear();
    
}

void Game::CheckFinalConditions() {

    if (player.hunger >= 100 || player.fatigue >= 100) {
        gameOver = true;
        ConsoleColors::SetColor(ConsoleColors::RED);
        std::wcout << L"\n������ �� �����������: �� ���� �� ������ ��� ���������.\n";
        ConsoleColors::Reset();
        return;
    }

    int rareMedalsCount = 0;
    int historicalMedalsCount = 0;
    for (const auto& m : player.inventory.GetItems()) {
        if (m.tier == MedalTier::Unique || m.tier == MedalTier::Valuable)
            rareMedalsCount++;
        if (m.effectOnPlayer.find(L"��������") != std::wstring::npos)
            historicalMedalsCount++;
    }

    if (player.money >= 100000 && rareMedalsCount >= 5 && player.reputation > 10) {
        gameOver = true;
        ConsoleColors::SetColor(ConsoleColors::GREEN);
        std::wcout << L"\n����� ����������: �� ������ ��� ������ ������ � ���� ����������� ����������.\n";
        ConsoleColors::Reset();
        return;
    }

    if (historicalMedalsCount >= 4 && player.reputation > 20 /* ������� */) {
        gameOver = true;
        ConsoleColors::SetColor(ConsoleColors::MAGENTA);
        std::wcout << L"\n���� ��������: �� ��� ������ � ����, ��� ��� �������, ��� ��������� ����������.\n";
        ConsoleColors::Reset();
        return;
    }

    for (const auto& m : player.inventory.GetItems()) {
        if (m.name.find(L"����� ��������") != std::wstring::npos) {
            if (rand() % 100 < 50) { // 50% ����
                gameOver = true;
                ConsoleColors::SetColor(ConsoleColors::YELLOW);
                std::wcout << L"\n������� �����: ������� ���������� ���� ������ ����� � ����� �� �������.\n";
                ConsoleColors::Reset();
                return;
            }
        }
    }

    if (day >= 30 && player.reputation < 0 && player.money < 500 && player.hunger > 80) {
        gameOver = true;
        ConsoleColors::SetColor(ConsoleColors::RED);
        std::wcout << L"\n������ �� �����������: ����� �� ������ �� ��������, ��������� �������� � �����.\n";
        ConsoleColors::Reset();
        return;
    }

    if (player.reputation > 15 && historicalMedalsCount >= 2) {
        gameOver = true;
        ConsoleColors::SetColor(ConsoleColors::CYAN);
        std::wcout << L"\n����� �����: ������ � ������� ���������� �� ������� ����� '������ � �����������'.\n";
        ConsoleColors::Reset();
        return;
    }

    if (day >= 15) {
        gameOver = true;
        ConsoleColors::SetColor(ConsoleColors::WHITE);
        std::wcout << L"\n�� � ������: ����� ������, ����� �������. �� ������ ����� �� ��������.\n";
        ConsoleColors::Reset();
        return;
    }

        

    if (player.hunger >= 100 || player.fatigue >= 100) {
        gameOver = true;
        ConsoleColors::SetColor(ConsoleColors::RED);
        std::wcout << L"\n�� ����. �� ������ ��� ���������. ��� � ����, � �������.\n";
        ConsoleColors::Reset();
    }

    if (player.money >= 1000000) {
        gameOver = true;
        ConsoleColors::SetColor(ConsoleColors::GREEN);
        std::wcout << L"\n�� ���������� �� �������. �������� � �������� �������������.\n";
        ConsoleColors::Reset();
    }

    if (player.reputation <= -50) {
        gameOver = true;
        ConsoleColors::SetColor(ConsoleColors::RED);
        std::wcout << L"\n���� ��������� ��������� ���������. ����� ������ �� ����� ����� � ����� ����.\n";
        ConsoleColors::Reset();
    }

    if (player.reputation >= 100) {
        gameOver = true;
        ConsoleColors::SetColor(ConsoleColors::GREEN);
        std::wcout << L"\n�� �������� �������� ����� ��������������! ������ �� ���������� �������.\n";
        ConsoleColors::Reset();
    }

    // ����� �������: ����������� �� ���� ��� ������������
    if (day > 10) {
        gameOver = true;
        ConsoleColors::SetColor(ConsoleColors::CYAN);
        std::wcout << L"\n������������ ��������� �� 10-� ���. �������� ����� ����:\n";
        ConsoleColors::Reset();

        // ��������� �����
        std::wcout << L"����: " << day << L"\n";
        std::wcout << L"������: " << player.money << L"\n";
        std::wcout << L"���������: " << player.reputation << L"\n";
        std::wcout << L"�����: " << player.hunger << L"\n";
        std::wcout << L"���������: " << player.fatigue << L"\n";
        std::wcout << L"���������:\n";
        player.ShowInventory();
        std::wcout << L"============================\n";
    }
}

void Game::ShowNPCs() const {
    if (currentLocation.npcs.empty()) {
        std::wcout << L"����� ������ ���.\n";
        return;
    }

    std::wcout << L"\n=== NPC � ���� ������� ===\n";
    for (size_t i = 0; i < currentLocation.npcs.size(); ++i) {
        std::wcout << i + 1 << L". " << currentLocation.npcs[i].name
            << L" - " << currentLocation.npcs[i].backstory << L"\n";
    }
}

void Game::InteractWithNPC() {
    ShowNPCs();
    if (currentLocation.npcs.empty()) return;

    std::wcout << L"������ NPC (1-" << currentLocation.npcs.size() << "): ";
    size_t choice;
    std::wcin >> choice;

    if (choice < 1 || choice > currentLocation.npcs.size()) {
        std::wcout << L"�������� �����!\n";
        return;
    }

    NPC& npc = currentLocation.npcs[choice - 1];

    if (npc.medalsForSale.empty()) {
        std::wcout << L"� " << npc.name << L" ��� ������� ��� �������.\n";
        return;
    }

    // �������� ������ NPC
    std::wcout << L"\n������ � " << npc.name << L":\n";
    for (size_t i = 0; i < npc.medalsForSale.size(); ++i) {
        const Medal& m = npc.medalsForSale[i];
        std::wcout << i + 1 << L". " << m.name << L" (" << m.condition << L") - "
            << m.minPrice << L"-" << m.maxPrice << L" ���.\n";
    }

    std::wcout << L"������ ������ ��� ������� (1-" << npc.medalsForSale.size() << "): ";
    size_t medalChoice;
    std::wcin >> medalChoice;

    if (medalChoice < 1 || medalChoice > npc.medalsForSale.size()) {
        std::wcout << L"�������� �����!\n";
        return;
    }

    Medal selectedMedal = npc.medalsForSale[medalChoice - 1];
    int price = (selectedMedal.minPrice + selectedMedal.maxPrice) / 2;

    // ����
    std::wcout << L"����: " << price << L" ���. ����������� �������������? (1-��, 0-���): ";
    int bargainChoice;
    std::wcin >> bargainChoice;

    if (bargainChoice == 1) {
        price = npc.Bargain(price, player.reputation * 0.01f);
        std::wcout << L"����� ���� ����� �����: " << price << L" ���.\n";
    }

    // ������� ��������
    std::wcout << L"����������� ��������? (1-��, 0-���): ";
    int cheatChoice;
    std::wcin >> cheatChoice;

    if (cheatChoice == 1) {
        if (npc.TryToCheat(player, selectedMedal)) {
            ConsoleColors::SetColor(ConsoleColors::MAGENTA);
            std::wcout << L"����� ������! �� ������� ������ ���������.\n";
            std::wcout << L"���������: " << player.reputation << L" (-5)\n";
            ConsoleColors::Reset();
            player.AddMedal(selectedMedal);
            return;
        }
        else {
            ConsoleColors::SetColor(ConsoleColors::RED);
            std::wcout << L"����� �������! " << npc.name << L" ����������.\n";
            std::wcout << L"���������: " << player.reputation << L" (-15)\n";
            ConsoleColors::Reset();
            price *= 2;
            std::wcout << L"������ ����: " << price << L" ���.\n";
        }
    }
    // �������
    if (player.money >= price) {
        player.money -= price;
        player.AddMedal(selectedMedal);
        npc.money += price;
        std::wcout << L"�� ����� " << selectedMedal.name << L" �� " << price << L" ���.\n";
    }
    else {
        std::wcout << L"� ���� ������������ �����!\n";
    }
}

