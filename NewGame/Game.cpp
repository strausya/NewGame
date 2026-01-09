
#include "Game.h"
#include <iostream>
#include "ConsoleColors.h"
#include "MedalDatabase.h"
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
        {L"Ежегодный взнос на развитие патриотического самосознания", 1400},
        {L"Плата за информационную безопасность граждан", 2200},
        {L"Сбор за упрощение административных процедур", 900},
        {L"Налог на цифровую трансформацию экономики", 1750},
        {L"Взнос на поддержку традиционных ценностей", 1300},
        {L"Платёж за интеграцию в национальную платежную систему", 800},
        {L"Налог на благоустройство и комфортную городскую среду", 1950},
        {L"Взнос на развитие искусственного интеллекта", 1650},
        {L"Сбор на создание единой базы данных обо всех", 2100},
        {L"Обязательный взнос в фонд модернизации ЖКХ", 2500}
    };
}

void Game::PrintAnimated(const std::wstring& text, int delayMs) {
    for (wchar_t c : text) {
        std::wcout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
    }
}

void Game::StartGame() {

    std::wcout << L"·····································································································" << std::endl;
    std::wcout << L": ________  ___  ___  ___  ________  _________  ___  ___       ________  ___  ___  ___  _________   :" << std::endl;
    std::wcout << L":|\\   ____\\|\\  \\|\\  \\|\\  \\|\\   ____\\|\\___   ___\\\\  \\|\\  \\     |\\   ____\\|\\  \\|\\  \\|\\  \\|\\___   ___\\ :" << std::endl;
    std::wcout << L":\\ \\  \\___|\\ \\  \\\\\\  \\ \\  \\ \\  \\___|\\|___ \\  \\_\\ \\  \\ \\  \\    \\ \\  \\___|\\ \\  \\\\\\  \\ \\  \\|___ \\  \\_| :" << std::endl;
    std::wcout << L": \\ \\  \\    \\ \\   __  \\ \\  \\ \\_____  \\   \\ \\  \\ \\ \\  \\ \\  \\    \\ \\_____  \\ \\   __  \\ \\  \\   \\ \\  \\  :" << std::endl;
    std::wcout << L":  \\ \\  \\____\\ \\  \\ \\  \\ \\  \\|____|\\  \\   \\ \\  \\ \\ \\  \\ \\  \\____\\|____|\\  \\ \\  \\ \\  \\ \\  \\   \\ \\  \\ :" << std::endl;
    std::wcout << L":   \\ \\_______\\ \\__\\ \\__\\ \\__\\____\\_\\  \\   \\ \\__\\ \\ \\__\\ \\_______\\____\\_\\  \\ \\__\\ \\__\\ \\__\\   \\ \\__\\:" << std::endl;
    std::wcout << L":    \\|_______|\\|__|\\|__|\\|__|\\_________\\   \\|__|  \\|__|\\|_______|\\_________\\|__|\\|__|\\|__|    \\|__|:" << std::endl;
    std::wcout << L":                            \\|_________|                        \\|_________|                       :" << std::endl;
    std::wcout << L"·························Игра называется Чистильщик································································" << std::endl;

    Beep(392, 300);  // Соль (G4)
    Beep(440, 300);  // Ля (A4)
    Beep(392, 300);  // Соль (G4)
    Beep(330, 600);  // Ми (E4) — длинная нота


    player.money = 30000;
    player.hunger = 0;
    player.fatigue = 0;
    player.reputation = 0;
    player.InitPrevStats();

    auto startingMedals = MedalDatabase::GetRandomMedals(5);
    for (const auto& medal : startingMedals) {
        player.AddMedal(medal);
    }

    PrintAnimated(L"Ты в Химках. У тебя нет денег. Только медали и отчаяние.\n");
    PrintAnimated(L"Нажал Enter, молодец, догадался. Уровень сложности: \n !!!!!!Ультра Хард!!!!!\n");
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

void Game::RenderUI() {

    if (skipUI) {
        skipUI = false;
    }

    ConsoleColors::SetColor(ConsoleColors::YELLOW);
    std::wcout << L"\n...........................................\n";

    ConsoleColors::SetColor(ConsoleColors::CYAN);
    std::wcout << L"=== День " << day << L" ===\n";
    std::wcout << weather.GetWeatherDescription() << L"\n";
    ConsoleColors::Reset();
    player.ShowStats();


    ConsoleColors::Reset();
    std::vector<std::wstring> actions = { L"Продавать медали \n2. Покупать медали \n3. Поесть \n 4. Поспать\n5. Сменить локацию \nЧто выбираешь?: " };
    std::wcout << L"\n=== Доступные действия ===\n";
    for (size_t i = 0; i < actions.size(); ++i) {
        std::wcout << i + 1 << L". " << actions[i] << L"\n";
    }

    int rareCount = 0;
    int historicalCount = 0;

    for (const auto& m : player.inventory.GetItems()) {
        if (m.tier == MedalTier::Unique || m.tier == MedalTier::Valuable) rareCount++;
        if (m.effectOnPlayer.find(L"Император") != std::wstring::npos ||
            m.effectOnPlayer.find(L"истор") != std::wstring::npos ||
            m.name.find(L"Орден") != std::wstring::npos) {
            historicalCount++;
        }
    }

    ConsoleColors::SetColor(ConsoleColors::GREEN);
    PrintAnimated(L"Локация: " + currentLocation.name + L"\n");
    ConsoleColors::Reset();


}

void Game::HandlePlayerChoice(int choice) {

    if (eventActive) {
        HandleEvent();
        return;
    }
    if (choice >= 1 && choice <= 5) {
        actionsToday++;

        // Проверка на перегруз
        if (actionsToday >= 10) {
                 NextDay();
            skipUI = false;
            return;

        }
    }

    switch (choice) {
    case 1:player.Trade(currentLocation);  break;
//player.EatFood(); skipUI = true; break;
    case 2:player.BuyFromNPC(currentLocation); break;
//player.Rest(); skipUI = true; break;
    case 3: player.EatFood(); skipUI = true; break;
        //player.Trade(currentLocation);  break;
    case 4:player.Rest(); skipUI = true; break;
        //player.BuyFromNPC(currentLocation); break;
    case 5: ChangeLocation(); skipUI = false; break;
        //ChangeLocation(); skipUI = false; break;
    //case 6: 
        //InteractWithNPC(); skipUI = true; break;
    //case 7:
        //NextDay();
        return;

    default: std::wcout << L"Неверный выбор!\n";
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

    if (day % 7 == 0 && !taxesList.empty()) {
        std::uniform_int_distribution<int> dist(0, static_cast<int>(taxesList.size()) - 1);
        const Tax& selectedTax = taxesList[dist(rng)];

        std::uniform_real_distribution<> variation(0.8, 1.2);
        int finalAmount = static_cast<int>(selectedTax.amount * variation(rng));

        player.money -= finalAmount;
        std::wcout << L"С вас снято " << finalAmount << L" руб. — " << selectedTax.name << L" (еженедельно).\n";
    }

    player.money -= utilitiesCost;
    if (day % 30 == 0) {
        std::wcout << L"С вас снято " << utilitiesCost << L" руб. за ЖКХ (ежемесячно).\n";
    }

    if (!taxesList.empty()) {
        std::uniform_int_distribution<int> dist(0, static_cast<int>(taxesList.size()) - 1);
        const Tax& selectedTax = taxesList[dist(rng)];

        std::uniform_real_distribution<> variation(0.8, 1.2);
        int finalAmount = static_cast<int>(selectedTax.amount * variation(rng));

        player.money -= finalAmount;
        std::wcout << L"С вас снято " << finalAmount << L" руб. — " << selectedTax.name << L".\n";
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

    actionsToday = 0;
    firstActionOfDay = true;
    ++day;
    if (day > MAX_DAY) {
        gameOver = true;
        std::wcout << L"\nДемонстрация закончена. Подводим итоги...\n";
        CheckFinalConditions();
        player.ShowStats();
        return;
    }

    actionsToday = 0;
    firstActionOfDay = true;
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

    player.ShowStats();
    prevActions.clear();

    CheckFinalConditions();

    ApplyWeatherEffects();

    if (day % 3 == 0) {
        currentEvents = currentLocation.GenerateDailyEvents(player);
        if (!currentEvents.empty()) eventActive = true;
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
        std::wcout << L"\nСмерть от одиночества: ты умер от голода или усталости.\n";
        ConsoleColors::Reset();
        return;
    }

    int rareMedalsCount = 0;
    int historicalMedalsCount = 0;
    for (const auto& m : player.inventory.GetItems()) {
        if (m.tier == MedalTier::Unique || m.tier == MedalTier::Valuable)
            rareMedalsCount++;
        if (m.effectOnPlayer.find(L"историей") != std::wstring::npos)
            historicalMedalsCount++;
    }

    if (player.money >= 100000 && rareMedalsCount >= 5 && player.reputation > 10) {
        gameOver = true;
        ConsoleColors::SetColor(ConsoleColors::GREEN);
        std::wcout << L"\nКнязь Химкинский: ты скупил все редкие медали и стал влиятельным персонажем.\n";
        ConsoleColors::Reset();
        return;
    }

    if (historicalMedalsCount >= 4 && player.reputation > 20 /* высокий */) {
        gameOver = true;
        ConsoleColors::SetColor(ConsoleColors::MAGENTA);
        std::wcout << L"\nШиза ветерана: ты так вжился в роль, что сам поверил, что штурмовал Кенигсберг.\n";
        ConsoleColors::Reset();
        return;
    }

    for (const auto& m : player.inventory.GetItems()) {
        if (m.name.find(L"Орден Мужества") != std::wstring::npos) {
            if (player.reputation >= 25) {
                gameOver = true;
                ConsoleColors::SetColor(ConsoleColors::YELLOW);
                std::wcout << L"\nЗолотой обмен: ты заработал доверие (репутация >= 25) и ветеран предлагает редкий обмен.\n";
                std::wcout << L"Почему так: финал зависит от твоей репутации и решений по честной торговле.\n";
                ConsoleColors::Reset();
                return;
            }
        }
    }
    if (day >= 30 && player.reputation < 0 && player.money < 500 && player.hunger > 80) {
        gameOver = true;
        ConsoleColors::SetColor(ConsoleColors::RED);
        std::wcout << L"\nСмерть от одиночества: никто не пришёл на похороны, коллекция осталась в сумке.\n";
        ConsoleColors::Reset();
        return;
    }

    if (player.reputation > 15 && historicalMedalsCount >= 2) {
        gameOver = true;
        ConsoleColors::SetColor(ConsoleColors::CYAN);
        std::wcout << L"\nЖивой музей: вместе с другими торговцами ты создаёшь музей 'Памяти и Перепродажи'.\n";
        ConsoleColors::Reset();
        return;
    }

    if (day >= 15) {
        gameOver = true;
        ConsoleColors::SetColor(ConsoleColors::WHITE);
        std::wcout << L"\nТы — медаль: экран гаснет, потом вспышка. Ты видишь себя… на прилавке.\n";
        ConsoleColors::Reset();
        return;
    }



    if (player.hunger >= 100 || player.fatigue >= 100) {
        gameOver = true;
        ConsoleColors::SetColor(ConsoleColors::RED);
        std::wcout << L"\nТы умер. От голода или усталости. Или и того, и другого.\n";
        ConsoleColors::Reset();
        return;
    }

    if (player.money >= 1000000) {
        gameOver = true;
        ConsoleColors::SetColor(ConsoleColors::GREEN);
        std::wcout << L"\nТы разбогател на медалях. Уважение в подъезде гарантировано.\n";
        ConsoleColors::Reset();
        return;
    }

    if (player.reputation <= -50) {
        ConsoleColors::SetColor(ConsoleColors::RED);
        std::wcout << L"\nТвоя репутация полностью разрушена. Никто больше не хочет иметь с тобой дело.\n";
        ConsoleColors::Reset();
        return;
    }

    if (player.reputation >= 100) {
        ConsoleColors::SetColor(ConsoleColors::GREEN);
        std::wcout << L"\nТы заслужил уважение среди коллекционеров! Теперь ты признанный эксперт.\n";
        ConsoleColors::Reset();
        return;
    }
}

static void RestockNPC(NPC& npc, int count = 5) {
    auto newMedals = MedalDatabase::GetRandomMedals(count);
    for (auto& m : newMedals) {
        npc.medalsForSale.push_back(m);
    }
}

