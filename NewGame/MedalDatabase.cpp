#include "MedalDatabase.h"
#include "Medal.h"

static const std::vector<Medal> medals = {
    {L"Медаль «За отвагу»", L"хорошая", 3000, 20000, MedalTier::Valuable, false,
     L"+Репутация: повышает шанс торга",
     100, MedalEffect::ReputationBoost, 3},

    {L"Медаль «Ветеран труда»", L"поношена", 200, 500, MedalTier::Budget, false,
     L"-20% к шансам торга",
     100, MedalEffect::ReputationBoost, -2},

    {L"Медаль «50 лет Победы в ВОВ»", L"новая", 100, 300, MedalTier::Budget, false,
     L"+Мемность: можно развести туриста",
     100, MedalEffect::MoneyBonus, 200},

    {L"Георгиевская медаль", L"хорошая", 15000, 90000, MedalTier::Valuable, false,
     L"+10% к торгу",
     100, MedalEffect::BargainBonus, 10},

    {L"Орден Святого Георгия IV степени", L"отличная", 250000, 1200000, MedalTier::Unique, false,
     L"Разблокирует квест: «Императорский след»",
     100, MedalEffect::ReputationBoost, 10},

    {L"Медаль «В память войны 1812 года»", L"удовлетворительная", 20000, 70000, MedalTier::Valuable, false,
     L"Повышенный шанс мошенничества",
     100, MedalEffect::None, 0},

    {L"Медаль «За турецкую войну 1877–1878 гг.»", L"удовлетворительная", 10000, 30000, MedalTier::Valuable, false,
     L"Интерес в кавказских локациях",
     100, MedalEffect::ReputationBoost, 2},

    {L"Орден Святой Анны III степени", L"хорошая", 30000, 150000, MedalTier::Valuable, true,
     L"Был статус — стал лот на Avito",
     100, MedalEffect::None, 0},

    {L"Медаль «Коронация Императора Николая II»", L"отличная", 15000, 40000, MedalTier::Valuable, true,
     L"Продаётся коллекционеру-монархисту",
     100, MedalEffect::MoneyBonus, 500},

    {L"Жетон «Общество поощрения трудолюбия»", L"поношен", 1000, 5000, MedalTier::Budget, false,
     L"Медаль за то, что не ленился в XIX веке. А ты ленишься и барыжишь",
     100, MedalEffect::None, 0},

    {L"Наградной крест «За усердие»", L"хорошая", 5000, 20000, MedalTier::Medium, false,
     L"Обход охраны",
     100, MedalEffect::FatigueRecovery, 3},

    {L"Медаль «За боевые заслуги»", L"средняя", 2000, 10000, MedalTier::Medium, false,
     L"+10% к цене у барыг",
     100, MedalEffect::MoneyBonus, 1000},

    {L"Медаль «За освобождение Праги»", L"удовлетворительная", 800, 2500, MedalTier::Budget, false,
     L"Один раз избежать облавы",
     100, MedalEffect::None, 0},

    {L"Медаль «60 лет Вооружённых сил СССР»", L"новая", 100, 300, MedalTier::Budget, false,
     L"+15% шанс уговорить купить шлак",
     100, MedalEffect::BargainBonus, 5},

    {L"Медаль «За трудовое отличие»", L"хорошая", 1500, 6000, MedalTier::Medium, false,
     L"Усталость растёт медленнее",
     100, MedalEffect::FatigueRecovery, 2},

    {L"Медаль «За доблестный труд»", L"поношена", 1000, 2000, MedalTier::Budget, false,
     L"Снимает дебафф 'стыд за жизнь'",
     100, MedalEffect::ReputationBoost, 1},

    {L"Медаль «За строительство БАМа»", L"средняя", 1500, 3500, MedalTier::Medium, false,
     L"—",
     100, MedalEffect::None, 0},

    {L"Медаль «За укрепление боевого содружества»", L"новая", 1000, 2500, MedalTier::Medium, false,
     L"Разблокирует VIP-контакт в Митино",
     100, MedalEffect::ReputationBoost, 4},

    {L"Медаль «За освоение целины»", L"удовлетворительная", 500, 1500, MedalTier::Budget, false,
     L"Привык питаться иллюзиями",
     100, MedalEffect::HungerReduction, 1},

    {L"Медаль «За победу над ЖКХ»", L"самодельная", 0, 0, MedalTier::Budget, true,
     L"Выжил зимой с отключённым отоплением. Герой нашего времени",
     100, MedalEffect::FatigueRecovery, 5},

    {L"Медаль «За успешную сдачу ЕГЭ»", L"блестящая", 100, 300, MedalTier::Budget, true,
     L"+5 к интеллекту",
     100, MedalEffect::ReputationBoost, 2},

    {L"Медаль «Лучший муж года»", L"новая", 150, 350, MedalTier::Budget, false,
     L"-10 к верности",
     100, MedalEffect::None, 0},

    {L"Орден Святого Владимира IV степени", L"отличная", 40000, 180000, MedalTier::Valuable, true,
     L"Уважение царистов",
     100, MedalEffect::ReputationBoost, 6},

    {L"Орден Святого Станислава III степени", L"хорошая", 20000, 80000, MedalTier::Medium, true,
     L"Часто покупают для понта",
     100, MedalEffect::MoneyBonus, 2000},

    {L"Орден Святой Екатерины (малая крестовая цепь)", L"роскошная", 2000000, 5000000, MedalTier::Unique, false,
     L"Только по связям",
     100, MedalEffect::ReputationBoost, 15}
};

const std::vector<Medal>& MedalDatabase::GetAllMedals() {
    return medals;
}

const Medal* MedalDatabase::FindByName(const std::wstring& name) {
    for (const auto& medal : medals) {
        if (medal.name == name)
            return &medal;
    }
    return nullptr;
}

std::vector<Medal> MedalDatabase::GetCheapestMedals(int count) {
    std::vector<Medal> sorted = medals;
    std::sort(sorted.begin(), sorted.end(), [](const Medal& a, const Medal& b) {
        return a.minPrice < b.minPrice;
        });
    if (count > (int)sorted.size()) count = (int)sorted.size();
    return std::vector<Medal>(sorted.begin(), sorted.begin() + count);
}

std::vector<Medal> MedalDatabase::GetRandomMedals(int count) {
    const auto& all = GetAllMedals();
    std::vector<Medal> result;
    if (all.empty()) return result;

    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_int_distribution<int> dist(0, (int)medals.size() - 1);

    for (int i = 0; i < count; ++i) {
        result.push_back(all[dist(g)]);
    }
    return result;
}