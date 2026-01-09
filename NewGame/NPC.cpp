#include "NPC.h"
#include "Player.h"
#include <algorithm>
#include <random>
#include <cmath>
#include "MedalDatabase.h"


template<typename T>
T ClampT(T x, T a, T b) {
    return (x < a) ? a : (x > b) ? b : x;
}

void NPC::AddMedal(const Medal& medal) {
    medalsForSale.push_back(medal);
}

void NPC::Restock(int count) {
    auto newMedals = MedalDatabase::GetRandomMedals(count);
    for (auto& m : newMedals) {
        medalsForSale.push_back(m);
    }
}

/*bool NPC::TryToCheat(Player& player, const Medal& medal) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    float successChance = gullibility * (1.0f - player.reputation * 0.01f);
    if (dis(gen) < successChance) {
        player.reputation -= 5;
        return true;
    }
    else {
        player.reputation -= 15;
        return false;
    }
}*/

int NPC::Bargain(int initialPrice, float playerSkill) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    float bargainEffect = playerSkill * (1.0f - bargainDifficulty);
    float discount = std::fmin(0.3f, bargainEffect * 0.3f); 

    return static_cast<int>(initialPrice * (1.0f - discount));
}

void NPC::InitializeDialogs() {
    dialogLines.clear();

    std::map<BargainTactic, std::vector<std::wstring>> baseDialogs = {
        {BargainTactic::BLUFF, {
            L"Я слышал, такие медали скоро изымут из оборота...",
            L"Только между нами - это же явная подделка, да?",
            L"Мне предлагали такую же дешевле в другом месте"
        }},
        {BargainTactic::FLATTERY, {
            L"Такой знаток как вы сразу видит настоящую ценность!",
            L"Для человека с вашим вкусом - особая цена!",
            L"Вы же понимаете толк в редких экземплярах!"
        }},
        {BargainTactic::THREAT, {
            L"Знаете, я могу и в полицию позвонить...",
            L"Мои друзья не оценят такой цены...",
            L"Вы ведь не хотите проблем?"
        }},
        {BargainTactic::REASON, {
            L"Давайте рассуждать логически...",
            L"По рыночной стоимости это стоит...",
            L"Я предлагаю справедливую цену"
        }},
        {BargainTactic::PATIENCE, {
            L"Я готов подождать...",
            L"Давайте не спешить...",
            L"Время работает на нас"
        }}
    };

    // Fix switch statement
    switch (type) {
    case NPCType::TRADER:
        baseDialogs[BargainTactic::THREAT] = {
            L"У меня есть знакомые в милиции...",
            L"Я тебе сейчас по морде дам!",
            L"Ты знаешь, с кем разговариваешь?"
        };
        baseDialogs[BargainTactic::FLATTERY] = {
            L"Не льсти, парень",
            L"Говори цену!",
            L"Лесть не поможет"
        };
        break;

    case NPCType::COLLECTOR:
        baseDialogs[BargainTactic::REASON] = {
            L"Интересный аргумент...",
            L"По каталогу эта медаль стоит...",
            L"Ваши доводы заслуживают внимания"
        };
        baseDialogs[BargainTactic::THREAT] = {
            L"Я вызываю охрану!",
            L"Это неприемлемо!",
            L"Вы переходите границы"
        };
        break;

    case NPCType::VETERAN:
        baseDialogs[BargainTactic::FLATTERY] = {
            L"В наше время ценили настоящих героев!",
            L"Молодежь сейчас не понимает ценность этих медалей",
            L"За Родину!"
        };
        baseDialogs[BargainTactic::THREAT] = {
            L"Я прошел войну, меня не испугаешь!",
            L"В мои годы такие шутки не прощают",
            L"Я тебя по-стариковски проучу!"
        };
        break;

    case NPCType::HOBBYIST:
        baseDialogs[BargainTactic::BLUFF] = {
            L"Правда? Я не знал...",
            L"Серьезно?",
            L"Ой, а я думал..."
        };
        baseDialogs[BargainTactic::FLATTERY] = {
            L"Вы правы, я хороший коллекционер!",
            L"Спасибо, я стараюсь!",
            L"Вы первый, кто это заметил!"
        };
        break;
    }
}

std::wstring NPC::GetDialogResponse(BargainTactic tactic, bool success) {
    static std::map<NPCType, std::map<BargainTactic, std::vector<std::wstring>>> successResponses = {
        {NPCType::TRADER, {
            {BargainTactic::BLUFF, {L"Ладно, убедил...", L"Хорошо, только для тебя!", L"Ты опасный собеседник!"}},
            {BargainTactic::FLATTERY, {L"Не льсти, но ладно", L"Говори цену быстрее", L"Давай по делу"}},
            {BargainTactic::THREAT, {L"Не надо нервничать", L"Ладно, не стоит ссориться", L"Ты выиграл этот раунд..."}},
            {BargainTactic::REASON, {L"Бизнес есть бизнес", L"Разумный аргумент", L"Справедливое предложение"}},
            {BargainTactic::PATIENCE, {L"Время - деньги", L"Ладно, не буду торговаться", L"Ты заслужил эту цену"}}
        }},
        {NPCType::COLLECTOR, {
            {BargainTactic::BLUFF, {L"Интересный блеф", L"Хорошо, я согласен", L"Ты убедил меня"}},
            {BargainTactic::FLATTERY, {L"Вы правы, я эксперт", L"Для такого знатока - особая цена", L"Лесть - твое оружие!"}},
            {BargainTactic::THREAT, {L"Это неприемлемо!", L"Я вызываю охрану!", L"Ты переходишь границы"}},
            {BargainTactic::REASON, {L"Твоя логика безупречна", L"Разумный аргумент", L"Справедливое предложение"}},
            {BargainTactic::PATIENCE, {L"Ты терпелив, я уступаю", L"Ладно, не буду торговаться", L"Ты заслужил эту цену"}}
        }},
        {NPCType::VETERAN, {
            {BargainTactic::BLUFF, {L"В наше время так не врали!", L"Ладно, старик поверил", L"Ты опасный собеседник!"}},
            {BargainTactic::FLATTERY, {L"Для ветерана сделаю исключение!", L"Спасибо за добрые слова", L"За Родину!"}},
            {BargainTactic::THREAT, {L"Я прошел войну!", L"Меня не испугаешь!", L"Ты выиграл этот раунд..."}},
            {BargainTactic::REASON, {L"В наше время так не считали", L"Разумный аргумент", L"Справедливое предложение"}},
            {BargainTactic::PATIENCE, {L"Мы умели ждать...", L"Ладно, не буду торговаться", L"Ты заслужил эту цену"}}
        }},
        {NPCType::HOBBYIST, {
            {BargainTactic::BLUFF, {L"Ой, правда?", L"Хорошо, я согласен", L"Ты убедил меня"}},
            {BargainTactic::FLATTERY, {L"Спасибо!", L"Я стараюсь!", L"Вы первый, кто это заметил!"}},
            {BargainTactic::THREAT, {L"Я... я пожалуй уйду...", L"Не надо...", L"Пожалуйста, не надо..."}},
            {BargainTactic::REASON, {L"А в интернете пишут...", L"Разумный аргумент", L"Справедливое предложение"}},
            {BargainTactic::PATIENCE, {L"Я подожду...", L"Ладно, не буду торговаться", L"Ты заслужил эту цену"}}
        }}
    };

    static std::map<NPCType, std::map<BargainTactic, std::vector<std::wstring>>> failResponses = {
        {NPCType::TRADER, {
            {BargainTactic::BLUFF, {L"Не лапши мне на уши!", L"Ты кого обманываешь?", L"Ну-ну, продолжай..."}},
            {BargainTactic::FLATTERY, {L"Я тебе что, девка на свидании?", L"Слюни убери", L"Не куплюсь"}},
            {BargainTactic::THREAT, {L"Сейчас сам испугаешься", L"Только попробуй!", L"Остынь"}},
            {BargainTactic::REASON, {L"Сказки мне не рассказывай", L"Не убедил", L"Это твои проблемы"}},
            {BargainTactic::PATIENCE, {L"Я и до ночи подожду", L"Упрёмся?", L"Не выйдет"}}
        }},
        {NPCType::COLLECTOR, {
            {BargainTactic::BLUFF, {L"Уровень дилетанта зашкаливает", L"Я знаю цену этой медали", L"Попробуй еще раз"}},
            {BargainTactic::FLATTERY, {L"Польщён, но сделка от этого не изменится", L"Слишком очевидно", L"Не повлияло"}},
            {BargainTactic::THREAT, {L"Это угроза? Я звоню", L"Смешно, но нет", L"Ты переходишь черту"}},
            {BargainTactic::REASON, {L"Аргументы хромают", L"Давай факты", L"Неубедительно"}},
            {BargainTactic::PATIENCE, {L"Я коллекционер, я жду годами", L"Спешка — враг сделки", L"Не тороплюсь"}}
        }},
        {NPCType::VETERAN, {
            {BargainTactic::BLUFF, {L"В сорок пятом так не врали", L"Не смеши фронтовика", L"Попробуй снова"}},
            {BargainTactic::FLATTERY, {L"Слова дешевле дел", L"Не подкупишь", L"Спасибо, но нет"}},
            {BargainTactic::THREAT, {L"Я и похуже видел", L"Не боюсь", L"Говори по делу"}},
            {BargainTactic::REASON, {L"Раньше так не считали", L"Аргумент слабый", L"Не убеждает"}},
            {BargainTactic::PATIENCE, {L"Мы умели ждать", L"Время у меня есть", L"Твоё ожидание меня не тронет"}}
        }},
        {NPCType::HOBBYIST, {
            {BargainTactic::BLUFF, {L"Серьёзно? Это правда?", L"Я не уверен...", L"Звучит странно"}},
            {BargainTactic::FLATTERY, {L"Ой, спасибо", L"Мне приятно", L"Может быть"}},
            {BargainTactic::THREAT, {L"Я... я ухожу", L"Не надо так", L"Пожалуйста, не кричи"}},
            {BargainTactic::REASON, {L"В интернете по-другому пишут", L"Не знаю...", L"Сомнительно"}},
            {BargainTactic::PATIENCE, {L"Я подожду", L"Ну ладно...", L"Долго ещё?"}}
        }}
    };

    const auto& responsesMap = success ? successResponses : failResponses;
    const auto& responses = responsesMap.at(type).at(tactic);

    if (responses.empty()) return L"...";

    static std::map<std::wstring, std::wstring> lastLineByKey;
    std::wstring key = name + L"|" + std::to_wstring((int)type) + L"|" + std::to_wstring((int)tactic) + L"|" + (success ? L"1" : L"0");

    std::wstring chosen = responses[rand() % responses.size()];
    if (!responses.empty()) {
        for (int i = 0; i < 6; ++i) {
            std::wstring candidate = responses[rand() % responses.size()];
            if (lastLineByKey[key] != candidate) {
                chosen = candidate;
                break;
            }
        }
    }

    lastLineByKey[key] = chosen;
    return chosen;
}


static float ClampF(float x, float a, float b) {
    if (x < a) return a;
    if (x > b) return b;
    return x;
}

float NPC::TacticMod(BargainTactic tactic, const NPC& npc) {
    switch (tactic) {
    case BargainTactic::BLUFF: {
        float m = 0.10f * npc.gullibility;
        if (npc.type == NPCType::COLLECTOR) m -= 0.08f;
        return m;
    }
    case BargainTactic::FLATTERY: {
        float m = 0.07f;
        if (npc.type == NPCType::HOBBYIST) m += 0.04f;
        return m;
    }
    case BargainTactic::THREAT:
        return (npc.type == NPCType::TRADER) ? 0.08f : -0.18f;
    case BargainTactic::REASON:
        return (npc.type == NPCType::COLLECTOR || npc.type == NPCType::VETERAN) ? 0.10f : 0.05f;
    case BargainTactic::PATIENCE:
        return 0.05f;
    default:
        return 0.0f;
    }
}

float NPC::CalculateTacticSuccessChance(BargainTactic tactic, const Player& player) const {
    auto ClampF = [](float x, float a, float b) { return (x < a) ? a : (x > b ? b : x); };

    float fatigue = ClampF(player.fatigue / 100.0f, 0.0f, 1.0f);
    float hunger = ClampF(player.hunger / 100.0f, 0.0f, 1.0f);

    float rep01 = ClampF(player.reputation / 100.0f, -1.0f, 1.0f);
    float trust01 = ClampF(player.GetTrust(this->name) / 100.0f, -1.0f, 1.0f);

    // База ниже, но репутация/доверие решают сильнее
    float base = 0.28f;

    // Репутация и доверие сильнее влияют, чтобы игрок "строил отношения"
    float repB = rep01 * 0.22f;
    float trustB = trust01 * 0.30f;

    // Сложность NPC режет сильнее
    float npcHard = -this->bargainDifficulty * 0.55f;

    // Устал/голоден — хуже торгуешься
    float needs = -(fatigue * 0.22f + hunger * 0.14f);

    // --- Тактика с БОЛЬШИМ РАЗБРОСОМ ---
    float tacticB = 0.0f;
    switch (tactic) {
    case BargainTactic::BLUFF:
        // сильная зависимость от gullibility и типа
        tacticB = 0.10f + 0.35f * this->gullibility;
        if (type == NPCType::COLLECTOR) tacticB -= 0.35f;
        if (type == NPCType::VETERAN)   tacticB -= 0.18f;
        break;

    case BargainTactic::FLATTERY:
        tacticB = 0.14f;
        if (type == NPCType::HOBBYIST)  tacticB += 0.12f;
        if (type == NPCType::TRADER)    tacticB -= 0.06f;
        break;

    case BargainTactic::THREAT:
        // очень полярная тактика
        tacticB = -0.10f;
        if (type == NPCType::TRADER)    tacticB = 0.30f;
        if (type == NPCType::COLLECTOR) tacticB = -0.42f;
        if (type == NPCType::VETERAN)   tacticB = -0.30f;
        if (type == NPCType::HOBBYIST)  tacticB = -0.35f;
        break;

    case BargainTactic::REASON:
        tacticB = 0.06f;
        if (type == NPCType::COLLECTOR) tacticB = 0.34f;
        if (type == NPCType::VETERAN)   tacticB = 0.22f;
        if (type == NPCType::TRADER)    tacticB = 0.10f;
        break;

    case BargainTactic::PATIENCE:
        // терпение зависит от твоего состояния и типа NPC
        tacticB = 0.08f - fatigue * 0.10f - hunger * 0.06f;
        if (type == NPCType::TRADER)    tacticB -= 0.08f;
        if (type == NPCType::COLLECTOR) tacticB += 0.06f;
        break;
    default:
        break;
    }

    float p = base + repB + trustB + npcHard + needs + tacticB;
    // широкий диапазон: иногда почти 0, иногда очень высоко
    return ClampF(p, 0.03f, 0.92f);
}