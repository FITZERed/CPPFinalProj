#include "ResourceUtils.h"

ResourceType StringToResource(const std::string& str) {
    if (str == "Spices") return ResourceType::Spices;
    if (str == "Textiles") return ResourceType::Textiles;
    if (str == "Jewelry") return ResourceType::Jewelry;
    if (str == "Minerals") return ResourceType::Minerals;
    if (str == "Medicine") return ResourceType::Medicine;
    return ResourceType::Spices; // Fallback
}

std::string ResourceToString(ResourceType type) {
    switch (type) {
    case ResourceType::Spices: return "Spices";
    case ResourceType::Textiles: return "Textiles";
    case ResourceType::Jewelry: return "Jewelry";
    case ResourceType::Minerals: return "Minerals";
    case ResourceType::Medicine: return "Medicine";
    default: return "Unknown";
    }
}