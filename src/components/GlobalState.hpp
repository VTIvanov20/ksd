#pragma once

#include "Component.hpp"
#include <unordered_map>
#include <optional>

class GlobalState : public Component
{
private:
    std::unordered_map<std::string, std::string> stateTable {};

public:
    DEFINE_COMPONENT(GlobalState)
    ~GlobalState() = default;

    std::optional<std::string> GetValue(std::string key);
    void SetValue(std::string key, std::string value);
    void DeleteValue(std::string key);

    bool DestroyOnReload() override { return false; };
};