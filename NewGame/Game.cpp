#include "Game.h"
#include <iostream>
#include "ConsoleColors.h"
#include <limits>
#include <string>

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
        {L"Р•Р¶РµРіРѕРґРЅС‹Р№ РІР·РЅРѕСЃ РЅР° СЂР°Р·РІРёС‚РёРµ РїР°С‚СЂРёРѕС‚РёС‡РµСЃРєРѕРіРѕ СЃР°РјРѕСЃРѕР·РЅР°РЅРёСЏ", 1400},
        {L"РџР»Р°С‚Р° Р·Р° РёРЅС„РѕСЂРјР°С†РёРѕРЅРЅСѓСЋ Р±РµР·РѕРїР°СЃРЅРѕСЃС‚СЊ РіСЂР°Р¶РґР°РЅ", 2200},
        {L"РЎР±РѕСЂ Р·Р° СѓРїСЂРѕС‰РµРЅРёРµ Р°РґРјРёРЅРёСЃС‚СЂР°С‚РёРІРЅС‹С… РїСЂРѕС†РµРґСѓСЂ", 900},
        {L"РќР°Р»РѕРі РЅР° С†РёС„СЂРѕРІСѓСЋ С‚СЂР°РЅСЃС„РѕСЂРјР°С†РёСЋ СЌРєРѕРЅРѕРјРёРєРё", 1750},
        {L"Р’Р·РЅРѕСЃ РЅР° РїРѕРґРґРµСЂР¶РєСѓ С‚СЂР°РґРёС†РёРѕРЅРЅС‹С… С†РµРЅРЅРѕСЃС‚РµР№", 1300},
        {L"РџР»Р°С‚С‘Р¶ Р·Р° РёРЅС‚РµРіСЂР°С†РёСЋ РІ РЅР°С†РёРѕРЅР°Р»СЊРЅСѓСЋ РїР»Р°С‚РµР¶РЅСѓСЋ СЃРёСЃС‚РµРјСѓ", 800},
        {L"РќР°Р»РѕРі РЅР° Р±Р»Р°РіРѕСѓСЃС‚СЂРѕР№СЃС‚РІРѕ Рё РєРѕРјС„РѕСЂС‚РЅСѓСЋ РіРѕСЂРѕРґСЃРєСѓСЋ СЃСЂРµРґСѓ", 1950},
        {L"Р’Р·РЅРѕСЃ РЅР° СЂР°Р·РІРёС‚РёРµ РёСЃРєСѓСЃСЃС‚РІРµРЅРЅРѕРіРѕ РёРЅС‚РµР»Р»РµРєС‚Р°", 1650},
        {L"РЎР±РѕСЂ РЅР° СЃРѕР·РґР°РЅРёРµ РµРґРёРЅРѕР№ Р±Р°Р·С‹ РґР°РЅРЅС‹С… РѕР±Рѕ РІСЃРµС…", 2100},
        {L"РћР±СЏР·Р°С‚РµР»СЊРЅС‹Р№ РІР·РЅРѕСЃ РІ С„РѕРЅРґ РјРѕРґРµСЂРЅРёР·Р°С†РёРё Р–РљРҐ", 2500}
    };
}

void Game::PrintAnimated(const std::wstring& text, int delayMs) {
    for (wchar_t c : text) {
        std::wcout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
    }
}

void Game::StartGame() {

    std::wcout << L"В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·" << std::endl;
    std::wcout << L": ________  ___  ___  ___  ________  _________  ___  ___       ________  ___  ___  ___  _________   :" << std::endl;
    std::wcout << L":|\\   ____\\|\\  \\|\\  \\|\\  \\|\\   ____\\|\\___   ___\\\\  \\|\\  \\     |\\   ____\\|\\  \\|\\  \\|\\  \\|\\___   ___\\ :" << std::endl;
    std::wcout << L":\\ \\  \\___|\\ \\  \\\\\\  \\ \\  \\ \\  \\___|\\|___ \\  \\_\\ \\  \\ \\  \\    \\ \\  \\___|\\ \\  \\\\\\  \\ \\  \\|___ \\  \\_| :" << std::endl;
    std::wcout << L": \\ \\  \\    \\ \\   __  \\ \\  \\ \\_____  \\   \\ \\  \\ \\ \\  \\ \\  \\    \\ \\_____  \\ \\   __  \\ \\  \\   \\ \\  \\  :" << std::endl;
    std::wcout << L":  \\ \\  \\____\\ \\  \\ \\  \\ \\  \\|____|\\  \\   \\ \\  \\ \\ \\  \\ \\  \\____\\|____|\\  \\ \\  \\ \\  \\ \\  \\   \\ \\  \\ :" << std::endl;
    std::wcout << L":   \\ \\_______\\ \\__\\ \\__\\ \\__\\____\\_\\  \\   \\ \\__\\ \\ \\__\\ \\_______\\____\\_\\  \\ \\__\\ \\__\\ \\__\\   \\ \\__\\:" << std::endl;
    std::wcout << L":    \\|_______|\\|__|\\|__|\\|__|\\_________\\   \\|__|  \\|__|\\|_______|\\_________\\|__|\\|__|\\|__|    \\|__|:" << std::endl;
    std::wcout << L":                            \\|_________|                        \\|_________|                       :" << std::endl;
    std::wcout << L"В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·В·" << std::endl;


    Beep(392, 300);  // РЎРѕР»СЊ (G4)
    Beep(440, 300);  // Р›СЏ (A4)
    Beep(392, 300);  // РЎРѕР»СЊ (G4)
    Beep(330, 600);  // РњРё (E4) вЂ” РґР»РёРЅРЅР°СЏ РЅРѕС‚Р°

    auto startingMedals = MedalDatabase::GetRandomMedals(1);

    for (const auto& medal : startingMedals) {
        player.AddMedal(medal);
      }

    PrintAnimated(L"Ты в Химках. У тебя нет денег. Только медали и отчаяние.\n");
    std::wcin.ignore((std::numeric_limits<std::streamsize>::max)(), L'\n');
    PrintAnimated(L"Нажал Enter, молодец, догадался. Уровень сложности: \n !!!!!!Ультра Хард!!!!!\n");
    std::wcin.ignore((std::numeric_limits<std::streamsize>::max)(), L'\n');
}

void Game::ChangeLocation() {
    ShowLocations();
    std::wcout << L"Куда пойдешь? (1-" << availableLocations.size() << "): ";

    int choice;
    std::wcin >> choice;

    if (choice >= 1 && choice <= static_cast<int>(availableLocations.size())) {
        currentLocation = availableLocations[choice - 1];
        PrintAnimated(L"Ты отправился в " + currentLocation.name + L"\n");
        PrintAnimated(currentLocation.GetTradeDescription() + L"\n");
    }
    else {
        std::wcout << L"Неверный выбор! Остаешься в текущей локации.\n";
    }
}

void Game::ShowLocations() const {
    std::wcout << L"\n=== Доступные локации ===\n";
    for (size_t i = 0; i < availableLocations.size(); ++i) {
        std::wcout << i + 1 << L". " << availableLocations[i].name << L"\n";
    }
}

void Game::ShowActionsIfChanged(const std::vector<std::wstring>& currentActions) {
    if (currentActions != prevActions) {
        std::wcout << L"\n=== Доступные действия ===\n";
        for (size_t i = 0; i < currentActions.size(); ++i) {
            std::wcout << i + 1 << L". " << currentActions[i] << L"\n";
        }
        prevActions = currentActions;
    }
}

void Game::RenderUI() {

    if (skipUI) {
        skipUI = false; // сброс
        return;
    }
    ConsoleColors::SetColor(ConsoleColors::YELLOW);
    std::wcout << L"\n...........................................\n";

    ConsoleColors::SetColor(ConsoleColors::CYAN);
    std::wcout << L"=== День " << day << L" ===\n";
    std::wcout << weather.GetWeatherDescription() << L"\n";
    ConsoleColors::Reset();
    //player.ShowChangedStats();
    player.ShowStats();

    ConsoleColors::SetColor(ConsoleColors::GREEN);
    PrintAnimated(L"Локация: " + currentLocation.name + L"\n");

    ConsoleColors::Reset();
        std::vector<std::wstring> actions = { L"1. Выпить кофе и поес \n2. Царственно отдыхать \n3. Торговать \n4. Сменить локацию \n5. Поговорить с NPC\n6. Закончить день\n Что выбираешь?: " };
        ShowActionsIfChanged(actions);

    if (actionsToday > 9) {
        ConsoleColors::SetColor(ConsoleColors::RED);
        std::wcout << L"Внимание! Следующее действие вызовет перегруз!\n";
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

            // Проверка на перегруз
            if (actionsToday > 10) {
                player.fatigue += 10;
                isOverworked = true;

                ConsoleColors::SetColor(ConsoleColors::RED);
                std::wcout << L"Слишком много действий! Усталость +10\n";
                ConsoleColors::Reset();
            }
        }
 
    switch (choice) {
    case 1: player.EatFood(); skipUI = true; break;
    case 2: player.Rest(); skipUI = true; break;
    case 3: player.Trade(currentLocation);  break;
    case 4: ChangeLocation(); skipUI = true; break;
    case 5: InteractWithNPC(); skipUI = true; break;
    case 6:
        NextDay();
        return; 
    default: std::wcout << L"Неверный выбор!\n";
    }

    if (!eventActive && actionsToday >= 5) {
        NextDay();
    }
}

void Game::ApplyWeatherEffects() {
    WeatherType currentWeather = weather.GetCurrentWeather();

    if (currentWeather == WeatherType::Rainy || currentWeather == WeatherType::Stormy) {
        player.fatigue += 5;
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
    // РЎРїРёСЃС‹РІР°РµРј Р–РљРҐ
    player.money -= utilitiesCost;
    std::wcout << L"РЎ РІР°СЃ СЃРЅСЏС‚Рѕ " << utilitiesCost << L" СЂСѓР±. Р·Р° Р–РљРҐ.\n";

    // РџРµСЂРµРјРµС€РёРІР°РµРј РЅР°Р»РѕРіРё
    std::shuffle(taxesList.begin(), taxesList.end(), rng);

    // РЎРїРёСЃС‹РІР°РµРј РЅР°Р»РѕРіРё СЃ СЂР°РЅРґРѕРјРёР·Р°С†РёРµР№ СЃСѓРјРјС‹
    std::uniform_real_distribution<> variation(0.8, 1.2);
    for (auto& tax : taxesList) {
        int finalAmount = static_cast<int>(tax.amount * variation(rng));
        player.money -= finalAmount;
        std::wcout << L"РЎ РІР°СЃ СЃРЅСЏС‚Рѕ " << finalAmount << L" СЂСѓР±. вЂ” " << tax.name << L".\n";
    }

    // РћРїР»Р°С‚Р° РєРІР°СЂС‚РёСЂС‹
    if (!rentPaid) {
        if (daysUntilEviction > 0) {
            std::wcout << L"РЈ РІР°СЃ РµСЃС‚СЊ " << daysUntilEviction
                << L" РґРµРЅСЊ, С‡С‚РѕР±С‹ Р·Р°РїР»Р°С‚РёС‚СЊ Р·Р° РєРІР°СЂС‚РёСЂСѓ: " << rentDue << L" СЂСѓР±.\n";
            daysUntilEviction--;
        }
        else {
            rentDue = static_cast<int>(rentDue * (1.0 + rentIncreaseRate));
            std::wcout << L"Р’С‹ РЅРµ Р·Р°РїР»Р°С‚РёР»Рё РІРѕРІСЂРµРјСЏ! Р”РѕР»Рі РІС‹СЂРѕСЃ РґРѕ " << rentDue << L" СЂСѓР±.\n";
        }
    }
}

void Game::NextDay() {
    weather.GenerateNewWeather();

    if (isOverworked) {
        player.fatigue += 30;
        player.reputation -= 5;
        ConsoleColors::SetColor(ConsoleColors::RED);
        std::wcout << L"Ты переработал вчера! Усталость +30, Репутация -5\n";
        ConsoleColors::Reset();
        isOverworked = false;
    }
       // Обычная логика дня
    actionsToday = 0;
    firstActionOfDay = true;
    ++day;
    player.hunger += 10;
    player.fatigue += 12; 
    
    ConsoleColors::SetColor(ConsoleColors::YELLOW);
    std::wcout << L"\n=== День " << day << L" ===\n";
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
   
    //player.ShowChangedStats();
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
    PrintAnimated(L"Произошло что!!!");
    ConsoleColors::Reset();

    for (const auto& event : currentEvents) {
        // Вывод описания события с цветом по типу
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

    std::wcout << L"\n1. Продолжить\n";
    if (currentEvents[0].type == EventType::Special) {
        std::wcout << L"2. Взять предмет\n";
    }
}

void Game::HandleEvent() {
    int choice;
    std::wcin >> choice;

    auto& event = currentEvents[0];
    switch (choice) {
    case 1: // Просто продолжаем
        break;
    case 2: // Для специальных событий
        if (event.type == EventType::Special && !event.items.empty()) {
            player.AddMedal(event.items[0]);
            std::wcout << L"Получена медаль: " << event.items[0].name << L"\n";
        }
        break;
    }

    // Применяем эффекты
    player.money += event.moneyEffect;
    player.reputation += event.reputationEffect;

    // Завершаем обработку события
    eventActive = false;
    currentEvents.clear();
    
}

void Game::CheckFinalConditions() {
    if (player.hunger >= 100 || player.fatigue >= 100) {
        gameOver = true;
        ConsoleColors::SetColor(ConsoleColors::RED);
        std::wcout << L"\nТы умер. От голода или усталости. Или и того, и другого.\n";
        ConsoleColors::Reset();
    }

    if (player.money >= 100000) {
        gameOver = true;
        ConsoleColors::SetColor(ConsoleColors::GREEN);
        std::wcout << L"\nТы разбогател на медалях. Уважение в подъезде гарантировано.\n";
        ConsoleColors::Reset();
    }

    if (player.reputation <= -50) {
        gameOver = true;
        ConsoleColors::SetColor(ConsoleColors::RED);
        std::wcout << L"\nТвоя репутация полностью разрушена. Никто больше не хочет иметь с тобой дело.\n";
        ConsoleColors::Reset();
    }

    if (player.reputation >= 100) {
        gameOver = true;
        ConsoleColors::SetColor(ConsoleColors::GREEN);
        std::wcout << L"\nТы заслужил уважение среди коллекционеров! Теперь ты признанный эксперт.\n";
        ConsoleColors::Reset();
    }

}

void Game::ShowNPCs() const {
    if (currentLocation.npcs.empty()) {
        std::wcout << L"Здесь никого нет.\n";
        return;
    }

    std::wcout << L"\n=== NPC в этой локации ===\n";
    for (size_t i = 0; i < currentLocation.npcs.size(); ++i) {
        std::wcout << i + 1 << L". " << currentLocation.npcs[i].name
            << L" - " << currentLocation.npcs[i].backstory << L"\n";
    }
}

void Game::InteractWithNPC() {
    ShowNPCs();
    if (currentLocation.npcs.empty()) return;

    std::wcout << L"Выбери NPC (1-" << currentLocation.npcs.size() << "): ";
    size_t choice;
    std::wcin >> choice;

    if (choice < 1 || choice > currentLocation.npcs.size()) {
        std::wcout << L"Неверный выбор!\n";
        return;
    }

    NPC& npc = currentLocation.npcs[choice - 1];

    if (npc.medalsForSale.empty()) {
        std::wcout << L"У " << npc.name << L" нет медалей для продажи.\n";
        return;
    }

    // Показать медали NPC
    std::wcout << L"\nМедали у " << npc.name << L":\n";
    for (size_t i = 0; i < npc.medalsForSale.size(); ++i) {
        const Medal& m = npc.medalsForSale[i];
        std::wcout << i + 1 << L". " << m.name << L" (" << m.condition << L") - "
            << m.minPrice << L"-" << m.maxPrice << L" руб.\n";
    }

    std::wcout << L"Выбери медаль для покупки (1-" << npc.medalsForSale.size() << "): ";
    size_t medalChoice;
    std::wcin >> medalChoice;

    if (medalChoice < 1 || medalChoice > npc.medalsForSale.size()) {
        std::wcout << L"Неверный выбор!\n";
        return;
    }

    Medal selectedMedal = npc.medalsForSale[medalChoice - 1];
    int price = (selectedMedal.minPrice + selectedMedal.maxPrice) / 2;

    // Торг
    std::wcout << L"Цена: " << price << L" руб. Попробовать поторговаться? (1-Да, 0-Нет): ";
    int bargainChoice;
    std::wcin >> bargainChoice;

    if (bargainChoice == 1) {
        price = npc.Bargain(price, player.reputation * 0.01f);
        std::wcout << L"Новая цена после торга: " << price << L" руб.\n";
    }

    // Попытка обмануть
    std::wcout << L"Попробовать обмануть? (1-Да, 0-Нет): ";
    int cheatChoice;
    std::wcin >> cheatChoice;

    if (cheatChoice == 1) {
        if (npc.TryToCheat(player, selectedMedal)) {
            ConsoleColors::SetColor(ConsoleColors::MAGENTA);
            std::wcout << L"Обман удался! Ты получил медаль бесплатно.\n";
            std::wcout << L"Репутация: " << player.reputation << L" (-5)\n";
            ConsoleColors::Reset();
            player.AddMedal(selectedMedal);
            return;
        }
        else {
            ConsoleColors::SetColor(ConsoleColors::RED);
            std::wcout << L"Обман раскрыт! " << npc.name << L" разозлился.\n";
            std::wcout << L"Репутация: " << player.reputation << L" (-15)\n";
            ConsoleColors::Reset();
            price *= 2;
            std::wcout << L"Теперь цена: " << price << L" руб.\n";
        }
    }
    // Покупка
    if (player.money >= price) {
        player.money -= price;
        player.AddMedal(selectedMedal);
        npc.money += price;
        std::wcout << L"Ты купил " << selectedMedal.name << L" за " << price << L" руб.\n";
    }
    else {
        std::wcout << L"У тебя недостаточно денег!\n";
    }
}

