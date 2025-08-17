#include "Location.h"
#include "MedalDatabase.h"
#include <random>
#include <ctime>
#include <cstdlib>
#include <map>

NPC::NPC(const std::wstring& name, const std::wstring& backstory, NPCType type,
    float bargainDifficulty, float gullibility, int money)
    : name(name), backstory(backstory), type(type),
    bargainDifficulty(bargainDifficulty), gullibility(gullibility), money(money) {
    InitializeDialogs();
}

void Location::AddNPC(const NPC& npc) {
    npcs.push_back(npc);
}

void Location::InitializeNPCs() {
    switch (type) {
    case LocationType::BaldRock:
        npcs.emplace_back(L"Дедушка Петрович", L"Бывший военный, коллекционирует медали с 1970-х", NPCType::VETERAN, 0.2f, 0.8f, 5000);
        npcs.emplace_back(L"Баба Зина", L"Продает медали своего покойного мужа", NPCType::TRADER, 0.3f, 0.7f, 3000);
        npcs.emplace_back(L"Светка", L"Красивая продавщица с местного рынка, любит приукрасить историю медали", NPCType::TRADER, 0.4f, 0.5f, 2500);
        npcs.emplace_back(L"Малой Тимка", L"Десятилетний пацан, притащил дедовскую медаль", NPCType::HOBBYIST, 0.1f, 0.6f, 500);
        break;

    case LocationType::GoldenSaffron:
        npcs.emplace_back(L"Олег Борисович", L"Бывший чиновник, разбирается в медалях", NPCType::COLLECTOR, 0.5f, 0.3f, 20000);
        npcs.emplace_back(L"Алексей", L"Молодой коллекционер, ищет редкие экземпляры", NPCType::HOBBYIST, 0.4f, 0.5f, 15000);
        npcs.emplace_back(L"Маргарита", L"Студентка-историк, интересуется наградами для курсовой", NPCType::HOBBYIST, 0.3f, 0.4f, 8000);
        npcs.emplace_back(L"Игорёк", L"Подросток с золотыми зубами, пытается толкнуть что-то сомнительное", NPCType::TRADER, 0.2f, 0.6f, 1200);
        break;

    case LocationType::MegaCollector:
        npcs.emplace_back(L"Сергей Иванович", L"Строгий коллекционер, знает всё о медалях", NPCType::COLLECTOR, 0.7f, 0.1f, 30000);
        npcs.emplace_back(L"Катя", L"Красивая девушка, торгует наградами в качестве хобби", NPCType::HOBBYIST, 0.3f, 0.4f, 5000);
        npcs.emplace_back(L"Артур", L"Подозрительный мужчина в кожаной куртке, перекупщик", NPCType::TRADER, 0.2f, 0.7f, 4000);
        break;

    case LocationType::Stakha:
        npcs.emplace_back(L"Дядя Ваня", L"Местный завсегдатай, любит торговаться", NPCType::TRADER, 0.1f, 0.9f, 1000);
        npcs.emplace_back(L"Коля", L"Парень с района, продает что попало", NPCType::HOBBYIST, 0.2f, 0.6f, 500);
        npcs.emplace_back(L"Даша", L"Симпатичная девушка в спортивном костюме, толкает медали 'по случаю'", NPCType::TRADER, 0.3f, 0.5f, 1500);
        npcs.emplace_back(L"Серёга Псина", L"Местный маргинал, пытается втюхать ржавое железо", NPCType::TRADER, 0.1f, 0.8f, 300);
        npcs.emplace_back(L"Ваня 'Школьник'", L"Подросток, сбежавший с уроков, торгует найденным на чердаке", NPCType::HOBBYIST, 0.1f, 0.5f, 700);
        break;
    }

    std::random_device rd;
    std::mt19937 gen(rd());

    for (auto& npc : npcs) {
        int medalsCount = 0;

        switch (npc.type) {
        case NPCType::COLLECTOR: {
            std::uniform_int_distribution<> dist(5, 8);
            medalsCount = dist(gen);
            break;
        }
        case NPCType::VETERAN: {
            std::uniform_int_distribution<> dist(1, 2);
            medalsCount = dist(gen);
            break;
        }
        case NPCType::TRADER: {
            std::uniform_int_distribution<> dist(3, 5);
            medalsCount = dist(gen);
            break;
        }
        case NPCType::HOBBYIST: {
            std::uniform_int_distribution<> dist(1, 3);
            medalsCount = dist(gen);
            break;
        }
        }

        auto medals = MedalDatabase::GetRandomMedals(medalsCount);

        for (auto& medal : medals) {
            if (npc.type == NPCType::TRADER) {
                std::uniform_int_distribution<> fakeChance(0, 99);
                if (fakeChance(gen) < 30) {
                    medal.isFake = true;
                }
            }
            if (npc.type == NPCType::VETERAN) {
                medal.tier = MedalTier::Unique;
            }
            npc.medalsForSale.push_back(medal);
        }
    }
}

Location::Location(LocationType locType) : type(locType) {
    switch (locType) {
    case LocationType::BaldRock:
        name = L"Лысый камень";
        tradeDescription = L"Дедушки покупают медали, но платят мало";
        priceModifier = 0.7f;
        fakeDetectionChance = 0.1f;
        bargainBonus = 0.0f;
        for (auto& npc : npcs) {
            auto randomMedals = MedalDatabase::GetRandomMedals(2 + rand() % 3);
            for (const auto& medal : randomMedals) {
                // С некоторой вероятностью медаль может быть фейком
                if (rand() % 100 < 30) {
                    Medal fakeMedal = medal;
                    fakeMedal.isFake = true;
                    npc.AddMedal(fakeMedal);
                }
                else {
                    npc.AddMedal(medal);
                }
            }
        }
        break;

    case LocationType::GoldenSaffron:
        name = L"Золотой Шафран";
        tradeDescription = L"Богатые покупатели, но могут проверить подлинность";
        priceModifier = 1.0f;
        fakeDetectionChance = 0.5f;
        bargainBonus = 0.15f;
        for (auto& npc : npcs) {
            auto randomMedals = MedalDatabase::GetRandomMedals(2 + rand() % 3);
            for (const auto& medal : randomMedals) {
                // С некоторой вероятностью медаль может быть фейком
                if (rand() % 100 < 30) {
                    Medal fakeMedal = medal;
                    fakeMedal.isFake = true;
                    npc.AddMedal(fakeMedal);
                }
                else {
                    npc.AddMedal(medal);
                }
            }
        }
        break;

    case LocationType::MegaCollector:
        name = L"Коллекционер в Меге";
        tradeDescription = L"Строгий коллекционер - платит много, но всё проверяет";
        priceModifier = 1.5f;
        fakeDetectionChance = 0.9f;
        bargainBonus = 0.0f;
        break;

    case LocationType::Stakha:
        name = L"Стаха";
        tradeDescription = L"Здесь можно выпросить деньги, но совсем немного";
        priceModifier = 0.5f;
        fakeDetectionChance = 0.0f;
        bargainBonus = 0.3f;
        break;
    }

    InitializeNPCs();
}

Location::Location(const std::wstring& name, const std::wstring& description) :
    name(name),
    tradeDescription(description),
    type(LocationType::BaldRock), // Default type
    priceModifier(1.0f),
    fakeDetectionChance(0.5f),
    bargainBonus(0.0f)
{
}

std::wstring Location::GetRandomEvent() const {
    static std::random_device rd;
    static std::mt19937 gen(rd());

    static std::map<LocationType, std::vector<std::wstring>> events = {
        {LocationType::BaldRock, {
            L"Дедушка рассказал тебе историю из своей молодости",
            L"Нашёл старую медаль в кармане",
            L"Кто-то предложил тебе чай",
            L"Случайно услышал спор о цене на редкую медаль",
            L"Пожилой мужчина похвалил твоё чувство юмора",
            L"Старушка показала фотографию своего мужа в молодости",
            L"Мимо прошёл человек с мешком непонятных металлических предметов",
            L"Ты увидел объявление о продаже 'оригинальных' медалей"
        }},
        {LocationType::GoldenSaffron, {
            L"В кафе подавали бесплатный чай",
            L"Ты встретил богатого коллекционера",
            L"Бариста спросил, коллекционируешь ли ты что-то",
            L"В углу сидел мужчина, рассматривающий медаль под лупой",
            L"Повар пожаловался на рост цен на специи",
            L"Официант намекнул, что видел 'такую же медаль' у соседа",
            L"Музыкант в углу играл мелодию времён Империи",
            L"Тебе предложили обменять ложку сахара на антикварную пуговицу"
        }},
        {LocationType::MegaCollector, {
            L"На аукционе сегодня скидки",
            L"Коллекционер в хорошем настроении",
            L"Кто-то продал подделку и был выведен охраной",
            L"Ты заметил медаль, похожую на одну из своих",
            L"Крупный торговец спорил с оценщиком",
            L"Случайно услышал секретную информацию о грядущей распродаже",
            L"Один из продавцов предложил тебе 'схему обогащения'",
            L"Оценщик пригласил тебя на частный просмотр коллекции"
        }},
        {LocationType::Stakha, {
            L"Кто-то оставил недопитый чай",
            L"Местные рассказывают байки",
            L"Старый друг предложил обменяться вещами",
            L"На стене висит объявление о пропавшей медали",
            L"Ты заметил подозрительного человека с полным рюкзаком",
            L"Тебя попросили оценить медаль, но она оказалась игрушечной",
            L"Женщина в платке предложила тебе 'очень выгодную сделку'",
            L"Ты увидел, как двое спорят о подлинности ордена"
        }}
    };

    const auto it = events.find(type);
    if (it != events.end()) {
        const auto& locEvents = it->second;
        std::uniform_int_distribution<int> dis(0, static_cast<int>(locEvents.size()) - 1);
        return locEvents[dis(gen)];
    }
    return L"Ничего не происходит.";
}

std::wstring Location::GetTradeDescription() const {
    switch (type) {
    case LocationType::BaldRock:
        return L"Дедушки покупают медали, но платят мало";
    case LocationType::GoldenSaffron:
        return L"Богатые покупатели, но могут проверить подлинность";
    case LocationType::MegaCollector:
        return L"Строгий коллекционер - платит много, но всё проверяет";
    case LocationType::Stakha:
        return L"Здесь можно выпросить деньги, но совсем немного";
    default:
        return L"Странное место для торговли...";
    }
}

std::vector<GameEvent> Location::GenerateDailyEvents() const {
    std::vector<GameEvent> events;
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> eventDist(0, 100);

    // Базовый шанс события (30%)
    if (eventDist(gen) > 70) return events;

    GameEvent event;
    std::uniform_int_distribution<> typeDist(0, 3);

    switch (typeDist(gen)) {
    case 0: // Нейтральное событие
        event.type = EventType::Neutral;
        event.description = GetRandomEvent();
        break;
    case 1: // Положительное
        event.type = EventType::Positive;
        event.description = L"Тебе повезло! " + GetRandomEvent();
        event.moneyEffect = 50 + (rand() % 200);
        break;
    case 2: // Отрицательное
        event.type = EventType::Negative;
        event.description = L"Неудача! " + GetRandomEvent();
        event.moneyEffect = -(20 + (rand() % 100));
        break;
    case 3: // Особое (с предметами)
        event.type = EventType::Special;
        event.description = L"Особый случай! " + GetRandomEvent();
        event.items = MedalDatabase::GetRandomMedals(1);
        break;
    }

    events.push_back(event);
    return events;
}



