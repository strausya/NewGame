#include "Inventory.h"
#include <iostream>

void Inventory::Add(const Medal& medal) {
    items.push_back(medal);
}

void Inventory::RemoveByIndex(size_t index) {
    if (index < items.size()) {
        items.erase(items.begin() + index);
    }
}

Medal Inventory::GetMedal(size_t index) const {
    if (index < items.size()) {
        return items[index];
    }
    // Return a default "unknown" medal with all required parameters
    return Medal(
        L"Неизвестная",   // name
        L"битая",         // condition
        0,                // minPrice
        0,                // maxPrice
        MedalTier::Budget,// tier
        true,             // isFake
        L"",              // effectOnPlayer
        0                 // baseCost
    );
}

size_t Inventory::Size() const {
    return items.size();
}

void Inventory::Print() const {
    if (items.empty()) {
        std::wcout << L"[Инвентарь пуст]\n";
        return;
    }
}

bool Inventory::IsEmpty() const {
    return items.empty();
}

const std::vector<Medal>& Inventory::GetItems() const {
    return items;
}