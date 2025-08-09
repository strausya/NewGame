#pragma once
#include "Medal.h"
#include <vector>

class Inventory {
private:
    std::vector<Medal> items;

public:
    void Add(const Medal& medal);
    void RemoveByIndex(size_t index);
    Medal GetMedal(size_t index) const;
    size_t Size() const;
    void Print() const;
    bool IsEmpty() const;
    const std::vector<Medal>& GetItems() const;
};

