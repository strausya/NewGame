#pragma once
#include "MedalManager.h"
#include <vector>


class MedalDatabase {

public:
    static const std::vector<Medal>& GetAllMedals();
    static const Medal* FindByName(const std::wstring& name);
    static std::vector<Medal> GetRandomMedals(int count);
};
