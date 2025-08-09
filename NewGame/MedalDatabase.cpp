#include "MedalDatabase.h"
#include "Medal.h"

static const std::vector<Medal> medals = {
    {L"Медаль «За отвагу»", L"хорошая", 3000, 20000, MedalTier::Valuable, false, L"+Репутация: повышает шанс торга", 100},
    {L"Медаль «Ветеран труда»", L"поношена", 200, 500, MedalTier::Budget, false, L"-20% к шансам торга", 100},
    {L"Медаль «50 лет Победы в ВОВ»", L"новая", 100, 300, MedalTier::Budget, false, L"+Мемность: можно развести туриста", 100},
    {L"Георгиевская медаль", L"хорошая", 15000, 90000, MedalTier::Valuable, false, L"+10% к торгу", 100},
    {L"Орден Святого Георгия IV степени", L"отличная", 250000, 1200000, MedalTier::Unique, false, L"разблокировать_квест:Императорский_след", 100},
    {L"Медаль «В память войны 1812 года»", L"удовлетворительная", 20000, 70000, MedalTier::Valuable, false, L"повышенный шанс мошенничества", 100},
    {L"Медаль «За турецкую войну 1877–1878 гг.»", L"удовлетворительная", 10000, 30000, MedalTier::Valuable, false, L"интерес в локациях с кавказским уклоном", 100},
    {L"Орден Святой Анны III степени", L"хорошая", 30000, 150000, MedalTier::Valuable, true, L"Был статус — стал лот на Avito", 100},
    {L"Медаль «Коронация Императора Николая II»", L"отличная", 15000, 40000, MedalTier::Valuable, true, L"продаётся коллекционеру монархической пошлости", 100},
    {L"Жетон «Общество поощрения трудолюбия»", L"поношен", 1000, 5000, MedalTier::Budget, false, L"Медаль за то, что не ленился в XIX веке. А ты ленишься и барыжишь", 100},
    {L"Наградной крест «За усердие»", L"хорошая", 5000, 20000, MedalTier::Medium, false, L"обход охраны", 100},
    {L"Медаль «За боевые заслуги»", L"средняя", 2000, 10000, MedalTier::Medium, false, L"+10% к цене у барыг", 100},
    {L"Медаль «За освобождение Праги»", L"удовлетворительная", 800, 2500, MedalTier::Budget, false, L"один раз избежать облавы", 100},
    {L"Медаль «60 лет Вооружённых сил СССР»", L"новая", 100, 300, MedalTier::Budget, false, L"+15% шанс уговорить купить шлак", 100},
    {L"Медаль «За трудовое отличие»", L"хорошая", 1500, 6000, MedalTier::Medium, false, L"усталость растёт медленнее", 100},
    {L"Медаль «За доблестный труд»", L"поношена", 1000, 2000, MedalTier::Budget, false, L"снимает дебафф 'стыд за жизнь'", 100},
    {L"Медаль «За строительство БАМа»", L"средняя", 1500, 3500, MedalTier::Medium, false, L"", 100},
    {L"Медаль «За укрепление боевого содружества»", L"новая", 1000, 2500, MedalTier::Medium, false, L"разблокирует VIP-контакт в Митино", 100},
    {L"Медаль «За освоение целины»", L"удовлетворительная", 500, 1500, MedalTier::Budget, false, L"привык питаться иллюзиями", 100},
    {L"Медаль «За победу над ЖКХ»", L"самодельная", 0, 0, MedalTier::Budget, true, L"Выжил зимой с отключённым отоплением. Герой нашего времени", 100},
    {L"Медаль «За успешную сдачу ЕГЭ»", L"блестящая", 100, 300, MedalTier::Budget, true, L"+5 к интеллекту", 100},
    {L"Медаль «Лучший муж года»", L"новая", 150, 350, MedalTier::Budget, false, L"-10 к верности", 100},
    {L"Орден Святого Владимира IV степени", L"отличная", 40000, 180000, MedalTier::Valuable, true, L"уважение царистов", 100},
    {L"Орден Святого Станислава III степени", L"хорошая", 20000, 80000, MedalTier::Medium, true, L"часто покупают для понта", 100},
    {L"Орден Святой Екатерины (малая крестовая цепь)", L"роскошная", 2000000, 5000000, MedalTier::Unique, false, L"только по связям", 100}
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

std::vector<Medal> MedalDatabase::GetRandomMedals(int count) {
    std::vector<Medal> result;
    const auto& allMedals = GetAllMedals();

    if (allMedals.empty()) return result;

    // Создаем копию для перемешивания
    std::vector<Medal> shuffled = allMedals;

    // Перемешиваем медали
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(shuffled.begin(), shuffled.end(), g);

    // Берем первые 'count' медалей
    int take = std::min(count, (int)shuffled.size());
    result.assign(shuffled.begin(), shuffled.begin() + take);

    return result;
}
