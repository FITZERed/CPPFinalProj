#pragma once
#include "Inventory.h"
#include <string>

ResourceType StringToResource(const std::string& str);
std::string ResourceToString(ResourceType type);