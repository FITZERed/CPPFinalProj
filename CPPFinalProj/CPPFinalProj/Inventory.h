#pragma once
enum class ResourceType {
    Spices,
    Textiles,
    Jewelry,
    Minerals,
    Medicine,
	Count            // This is used as a trick to get the number of resource types (ResourceType::Count will be 5)
};

class Inventory {
public:
    Inventory();

    int money;
    int resourceCounts[(int)ResourceType::Count];

    void Add(ResourceType, int);
    void Remove(ResourceType, int);
    int GetAmount(ResourceType) const;
};