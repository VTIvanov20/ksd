#pragma once

#include "Component.hpp"
#include <unordered_map>
#include <optional>

class GlobalState : public Component
{
private:
    std::unordered_map<std::string, char> stateTable {};

public:
    DEFINE_COMPONENT(GlobalState)
    ~GlobalState() = default;

    std::optional<char> GetValue(std::string key);
    void SetValue(std::string key, char value);
    void DeleteValue(std::string key);

    bool DestroyOnReload = false;
};