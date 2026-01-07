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
        {L"Åæåãîäíûé âçíîñ íà ðàçâèòèå ïàòðèîòè÷åñêîãî ñàìîñîçíàíèÿ", 1400},
        {L"Ïëàòà çà èíôîðìàöèîííóþ áåçîïàñíîñòü ãðàæäàí", 2200},
        {L"Ñáîð çà óïðîùåíèå àäìèíèñòðàòèâíûõ ïðîöåäóð", 900},
        {L"Íàëîã íà öèôðîâóþ òðàíñôîðìàöèþ ýêîíîìèêè", 1750},
        {L"Âçíîñ íà ïîääåðæêó òðàäèöèîííûõ öåííîñòåé", 1300},
        {L"Ïëàò¸æ çà èíòåãðàöèþ â íàöèîíàëüíóþ ïëàòåæíóþ ñèñòåìó", 800},
        {L"Íàëîã íà áëàãîóñòðîéñòâî è êîìôîðòíóþ ãîðîäñêóþ ñðåäó", 1950},
        {L"Âçíîñ íà ðàçâèòèå èñêóññòâåííîãî èíòåëëåêòà", 1650},
        {L"Ñáîð íà ñîçäàíèå åäèíîé áàçû äàííûõ îáî âñåõ", 2100},
        {L"Îáÿçàòåëüíûé âçíîñ â ôîíä ìîäåðíèçàöèè ÆÊÕ", 2500}
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
    std::wcout << L"·························Èãðà íàçûâàåòñÿ ×èñòèëüùèê································································" << std::endl;





    Beep(392, 300);  // Ñîëü (G4)
    Beep(440, 300);  // Ëÿ (A4)
    Beep(392, 300);  // Ñîëü (G4)
    Beep(330, 600);  // Ìè (E4) — äëèííàÿ íîòà

    auto startingMedals = MedalDatabase::GetCheapestMedals(10);
    for (const auto& medal : startingMedals) {
        player.AddMedal(medal);
    }

    for (const auto& medal : startingMedals) {
        player.AddMedal(medal);
      }

    PrintAnimated(L"Òû â Õèìêàõ. Ó òåáÿ íåò äåíåã. Òîëüêî ìåäàëè è îò÷àÿíèå.\n");
    std::wcin.ignore((std::numeric_limits<std::streamsize>::max)(), L'\n');
    PrintAnimated(L"Íàæàë Enter, ìîëîäåö, äîãàäàëñÿ. Óðîâåíü ñëîæíîñòè: \n !!!!!!Óëüòðà Õàðä!!!!!\n");
    std::wcin.ignore((std::numeric_limits<std::streamsize>::max)(), L'\n');
}

void Game::ChangeLocation() {
    ShowLocations();
    std::wcout << L"Êóäà ïîéäåøü? (1-" << availableLocations.size() << "): ";

    int choice;
    std::wcin >> choice;

    if (choice >= 1 && choice <= static_cast<int>(availableLocations.size())) {
        currentLocation = availableLocations[choice - 1];
        PrintAnimated(L"Òû îòïðàâèëñÿ â " + currentLocation.name + L"\n");
        PrintAnimated(currentLocation.GetTradeDescription() + L"\n");
    }
    else {
        std::wcout << L"Íåâåðíûé âûáîð! Îñòàåøüñÿ â òåêóùåé ëîêàöèè.\n";
    }
}

void Game::ShowLocations() const {
    std::wcout << L"\n=== Äîñòóïíûå ëîêàöèè ===\n";
    for (size_t i = 0; i < availableLocations.size(); ++i) {
        std::wcout << i + 1 << L". " << availableLocations[i].name << L"\n";
    }
}

void Game::RenderUI() {

    if (skipUI) {
        skipUI = false; // ñáðîñ
        return;
    }
    ConsoleColors::SetColor(ConsoleColors::YELLOW);
    std::wcout << L"\n...........................................\n";

    ConsoleColors::SetColor(ConsoleColors::CYAN);
    std::wcout << L"=== Äåíü " << day << L" ===\n";
    std::wcout << weather.GetWeatherDescription() << L"\n";
    ConsoleColors::Reset();
    //player.ShowChangedStats();
    player.ShowStats();

    ConsoleColors::SetColor(ConsoleColors::GREEN);
    PrintAnimated(L"Ëîêàöèÿ: " + currentLocation.name + L"\n");

    ConsoleColors::Reset();
        std::vector<std::wstring> actions = { L"Âûïèòü êîôå è ïîåñ \n2. Ïîñïàòü â êîðîáî÷êå. \n3. Òîðãîâàòü \n 4. ÏÎÊÓÏÀÒÜ \n 5. Ñìåíèòü ëîêàöèþ \n6. Ïîãîâîðèòü ñ NPC\n7. Çàêîí÷èòü äåíü\n ×òî âûáèðàåøü?: " };
        std::wcout << L"\n=== Äîñòóïíûå äåéñòâèÿ ===\n";
        for (size_t i = 0; i < actions.size(); ++i) {
            std::wcout << i + 1 << L". " << actions[i] << L"\n";
        }

    if (actionsToday > 9) {
        ConsoleColors::SetColor(ConsoleColors::RED);
        std::wcout << L"Âíèìàíèå! Ñëåäóþùåå äåéñòâèå âûçîâåò ïåðåãðóç!\n";
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

            // Ïðîâåðêà íà ïåðåãðóç
            if (actionsToday > 10) {
                player.fatigue += 10;
                isOverworked = true;

                ConsoleColors::SetColor(ConsoleColors::RED);
                std::wcout << L"Ñëèøêîì ìíîãî äåéñòâèé! Óñòàëîñòü +10\n";
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

    if (skipFirstPayment) {
        skipFirstPayment = false;
        return;
    }

    default: std::wcout << L"Íåâåðíûé âûáîð!\n";
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

    // Ñïèñûâàåì ÆÊÕ
    player.money -= utilitiesCost;
    std::wcout << L"Ñ âàñ ñíÿòî " << utilitiesCost << L" ðóá. çà ÆÊÕ.\n";

    // Âûáèðàåì ñëó÷àéíûé íàëîã
    if (!taxesList.empty()) {
        std::uniform_int_distribution<int> dist(0, static_cast<int>(taxesList.size()) - 1);
        const Tax& selectedTax = taxesList[dist(rng)];

        // Ðàíäîìèçèðóåì ñóììó íàëîãà
        std::uniform_real_distribution<> variation(0.8, 1.2);
        int finalAmount = static_cast<int>(selectedTax.amount * variation(rng));

        player.money -= finalAmount;
        std::wcout << L"Ñ âàñ ñíÿòî " << finalAmount << L" ðóá. — " << selectedTax.name << L".\n";
    }

    // Îïëàòà êâàðòèðû
    if (!rentPaid) {
        if (daysUntilEviction > 0) {
            std::wcout << L"Ó âàñ åñòü " << daysUntilEviction
                << L" äåíü, ÷òîáû çàïëàòèòü çà êâàðòèðó: " << rentDue << L" ðóá.\n";
            daysUntilEviction--;
        }
        else {
            rentDue = static_cast<int>(rentDue * (1.0 + rentIncreaseRate));
            std::wcout << L"Âû íå çàïëàòèëè âîâðåìÿ! Äîëã âûðîñ äî " << rentDue << L" ðóá.\n";
        }
    }
}

void Game::NextDay() {
    weather.GenerateNewWeather();


    if (isOverworked) {
        player.fatigue += 30;
        player.reputation -= 5;
        ConsoleColors::SetColor(ConsoleColors::RED);
        std::wcout << L"Òû ïåðåðàáîòàë â÷åðà! Óñòàëîñòü +30, Ðåïóòàöèÿ -5\n";
        ConsoleColors::Reset();
        isOverworked = false;
    }

    actionsToday = 0;
    firstActionOfDay = true;
    ++day;
    if (day > MAX_DAY) {
        gameOver = true;
        std::wcout << L"\nÄåìîíñòðàöèÿ çàêîí÷åíà. Ïîäâîäèì èòîãè...\n";
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
    std::wcout << L"\n=== Äåíü " << day << L" ===\n";
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
    PrintAnimated(L"Ïðîèçîøëî ÷òî!!!");
    ConsoleColors::Reset();

    for (const auto& event : currentEvents) {
        // Âûâîä îïèñàíèÿ ñîáûòèÿ ñ öâåòîì ïî òèïó
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

    std::wcout << L"\n1. Ïðîäîëæèòü\n";
    if (currentEvents[0].type == EventType::Special) {
        std::wcout << L"2. Âçÿòü ïðåäìåò\n";
    }
}

void Game::HandleEvent() {
    int choice;
    std::wcin >> choice;

    auto& event = currentEvents[0];
    switch (choice) {
    case 1: // Ïðîñòî ïðîäîëæàåì
        break;
    case 2: // Äëÿ ñïåöèàëüíûõ ñîáûòèé
        if (event.type == EventType::Special && !event.items.empty()) {
            player.AddMedal(event.items[0]);
            std::wcout << L"Ïîëó÷åíà ìåäàëü: " << event.items[0].name << L"\n";
        }
        break;
    }

    // Ïðèìåíÿåì ýôôåêòû
    player.money += event.moneyEffect;
    player.reputation += event.reputationEffect;

    // Çàâåðøàåì îáðàáîòêó ñîáûòèÿ
    eventActive = false;
    currentEvents.clear();
    
}

void Game::CheckFinalConditions() {

    if (player.hunger >= 100 || player.fatigue >= 100) {
        gameOver = true;
        ConsoleColors::SetColor(ConsoleColors::RED);
        std::wcout << L"\nÑìåðòü îò îäèíî÷åñòâà: òû óìåð îò ãîëîäà èëè óñòàëîñòè.\n";
        ConsoleColors::Reset();
        return;
    }

    int rareMedalsCount = 0;
    int historicalMedalsCount = 0;
    for (const auto& m : player.inventory.GetItems()) {
        if (m.tier == MedalTier::Unique || m.tier == MedalTier::Valuable)
            rareMedalsCount++;
        if (m.effectOnPlayer.find(L"èñòîðèåé") != std::wstring::npos)
            historicalMedalsCount++;
    }

    if (player.money >= 100000 && rareMedalsCount >= 5 && player.reputation > 10) {
        gameOver = true;
        ConsoleColors::SetColor(ConsoleColors::GREEN);
        std::wcout << L"\nÊíÿçü Õèìêèíñêèé: òû ñêóïèë âñå ðåäêèå ìåäàëè è ñòàë âëèÿòåëüíûì ïåðñîíàæåì.\n";
        ConsoleColors::Reset();
        return;
    }

    if (historicalMedalsCount >= 4 && player.reputation > 20 /* âûñîêèé */) {
        gameOver = true;
        ConsoleColors::SetColor(ConsoleColors::MAGENTA);
        std::wcout << L"\nØèçà âåòåðàíà: òû òàê âæèëñÿ â ðîëü, ÷òî ñàì ïîâåðèë, ÷òî øòóðìîâàë Êåíèãñáåðã.\n";
        ConsoleColors::Reset();
        return;
    }

    for (const auto& m : player.inventory.GetItems()) {
        if (m.name.find(L"Îðäåí Ìóæåñòâà") != std::wstring::npos) {
            if (rand() % 100 < 50) { // 50% øàíñ
                gameOver = true;
                ConsoleColors::SetColor(ConsoleColors::YELLOW);
                std::wcout << L"\nÇîëîòîé îáìåí: âåòåðàí ïðåäëàãàåò òåáå ðåäêèé îðäåí â îáìåí íà ïàñïîðò.\n";
                ConsoleColors::Reset();
                return;
            }
        }
    }

    if (day >= 30 && player.reputation < 0 && player.money < 500 && player.hunger > 80) {
        gameOver = true;
        ConsoleColors::SetColor(ConsoleColors::RED);
        std::wcout << L"\nÑìåðòü îò îäèíî÷åñòâà: íèêòî íå ïðèø¸ë íà ïîõîðîíû, êîëëåêöèÿ îñòàëàñü â ñóìêå.\n";
        ConsoleColors::Reset();
        return;
    }

    if (player.reputation > 15 && historicalMedalsCount >= 2) {
        gameOver = true;
        ConsoleColors::SetColor(ConsoleColors::CYAN);
        std::wcout << L"\nÆèâîé ìóçåé: âìåñòå ñ äðóãèìè òîðãîâöàìè òû ñîçäà¸øü ìóçåé 'Ïàìÿòè è Ïåðåïðîäàæè'.\n";
        ConsoleColors::Reset();
        return;
    }

    if (day >= 15) {
        gameOver = true;
        ConsoleColors::SetColor(ConsoleColors::WHITE);
        std::wcout << L"\nÒû — ìåäàëü: ýêðàí ãàñíåò, ïîòîì âñïûøêà. Òû âèäèøü ñåáÿ… íà ïðèëàâêå.\n";
        ConsoleColors::Reset();
        return;
    }

        

    if (player.hunger >= 100 || player.fatigue >= 100) {
        gameOver = true;
        ConsoleColors::SetColor(ConsoleColors::RED);
        std::wcout << L"\nÒû óìåð. Îò ãîëîäà èëè óñòàëîñòè. Èëè è òîãî, è äðóãîãî.\n";
        ConsoleColors::Reset();
    }

    if (player.money >= 1000000) {
        gameOver = true;
        ConsoleColors::SetColor(ConsoleColors::GREEN);
        std::wcout << L"\nÒû ðàçáîãàòåë íà ìåäàëÿõ. Óâàæåíèå â ïîäúåçäå ãàðàíòèðîâàíî.\n";
        ConsoleColors::Reset();
    }

    if (player.reputation <= -50) {
        gameOver = true;
        ConsoleColors::SetColor(ConsoleColors::RED);
        std::wcout << L"\nÒâîÿ ðåïóòàöèÿ ïîëíîñòüþ ðàçðóøåíà. Íèêòî áîëüøå íå õî÷åò èìåòü ñ òîáîé äåëî.\n";
        ConsoleColors::Reset();
    }

    if (player.reputation >= 100) {
        gameOver = true;
        ConsoleColors::SetColor(ConsoleColors::GREEN);
        std::wcout << L"\nÒû çàñëóæèë óâàæåíèå ñðåäè êîëëåêöèîíåðîâ! Òåïåðü òû ïðèçíàííûé ýêñïåðò.\n";
        ConsoleColors::Reset();
    }
}

void Game::ShowNPCs() const {
    if (currentLocation.npcs.empty()) {
        std::wcout << L"Çäåñü íèêîãî íåò.\n";
        return;
    }

    std::wcout << L"\n=== NPC â ýòîé ëîêàöèè ===\n";
    for (size_t i = 0; i < currentLocation.npcs.size(); ++i) {
        std::wcout << i + 1 << L". " << currentLocation.npcs[i].name
            << L" - " << currentLocation.npcs[i].backstory << L"\n";
    }
}

void Game::InteractWithNPC() {
    ShowNPCs();
    if (currentLocation.npcs.empty()) return;

    std::wcout << L"Âûáåðè NPC (1-" << currentLocation.npcs.size() << "): ";
    size_t choice;
    std::wcin >> choice;

    if (choice < 1 || choice > currentLocation.npcs.size()) {
        std::wcout << L"Íåâåðíûé âûáîð!\n";
        return;
    }

    NPC& npc = currentLocation.npcs[choice - 1];

    if (npc.medalsForSale.empty()) {
        std::wcout << L"Ó " << npc.name << L" íåò ìåäàëåé äëÿ ïðîäàæè.\n";
        return;
    }

    // Ïîêàçàòü ìåäàëè NPC
    std::wcout << L"\nÌåäàëè ó " << npc.name << L":\n";
    for (size_t i = 0; i < npc.medalsForSale.size(); ++i) {
        const Medal& m = npc.medalsForSale[i];
        std::wcout << i + 1 << L". " << m.name << L" (" << m.condition << L") - "
            << m.minPrice << L"-" << m.maxPrice << L" ðóá.\n";
    }

    std::wcout << L"Âûáåðè ìåäàëü äëÿ ïîêóïêè (1-" << npc.medalsForSale.size() << "): ";
    size_t medalChoice;
    std::wcin >> medalChoice;

    if (medalChoice < 1 || medalChoice > npc.medalsForSale.size()) {
        std::wcout << L"Íåâåðíûé âûáîð!\n";
        return;
    }

    Medal selectedMedal = npc.medalsForSale[medalChoice - 1];
    int price = (selectedMedal.minPrice + selectedMedal.maxPrice) / 2;

    // Òîðã
    std::wcout << L"Öåíà: " << price << L" ðóá. Ïîïðîáîâàòü ïîòîðãîâàòüñÿ? (1-Äà, 0-Íåò): ";
    int bargainChoice;
    std::wcin >> bargainChoice;

    if (bargainChoice == 1) {
        price = npc.Bargain(price, player.reputation * 0.01f);
        std::wcout << L"Íîâàÿ öåíà ïîñëå òîðãà: " << price << L" ðóá.\n";
    }

    // Ïîïûòêà îáìàíóòü
    std::wcout << L"Ïîïðîáîâàòü îáìàíóòü? (1-Äà, 0-Íåò): ";
    int cheatChoice;
    std::wcin >> cheatChoice;

    if (cheatChoice == 1) {
        if (npc.TryToCheat(player, selectedMedal)) {
            ConsoleColors::SetColor(ConsoleColors::MAGENTA);
            std::wcout << L"Îáìàí óäàëñÿ! Òû ïîëó÷èë ìåäàëü áåñïëàòíî.\n";
            std::wcout << L"Ðåïóòàöèÿ: " << player.reputation << L" (-5)\n";
            ConsoleColors::Reset();
            player.AddMedal(selectedMedal);
            return;
        }
        else {
            ConsoleColors::SetColor(ConsoleColors::RED);
            std::wcout << L"Îáìàí ðàñêðûò! " << npc.name << L" ðàçîçëèëñÿ.\n";
            std::wcout << L"Ðåïóòàöèÿ: " << player.reputation << L" (-15)\n";
            ConsoleColors::Reset();
            price *= 2;
            std::wcout << L"Òåïåðü öåíà: " << price << L" ðóá.\n";
        }
    }
    // Ïîêóïêà
    if (player.money >= price) {
        player.money -= price;
        player.AddMedal(selectedMedal);
        npc.money += price;
        std::wcout << L"Òû êóïèë " << selectedMedal.name << L" çà " << price << L" ðóá.\n";
    }
    else {
        std::wcout << L"Ó òåáÿ íåäîñòàòî÷íî äåíåã!\n";
    }
}

