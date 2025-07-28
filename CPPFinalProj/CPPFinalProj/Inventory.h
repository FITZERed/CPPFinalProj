#pragma once
enum class ResourceType {
    Spices,
    Textiles,
    Jewelry,
    Minerals,
    Medicine,
    Count
};

class Inventory {
public:
    int money;
    int resourceCounts[(int)ResourceType::Count];

    void Add(ResourceType, int);
    void Remove(ResourceType, int);
    int GetAmount(ResourceType);
};