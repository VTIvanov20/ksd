#include "GlobalState.hpp"
#include <string>
#include <optional>

std::optional<std::string> GlobalState::GetValue(std::string key)
{
    if (stateTable.find(key) != stateTable.end())
        return stateTable[key];
    else return {};
}

void GlobalState::SetValue(std::string key, std::string value)
{
    stateTable[key] = value;
}

void GlobalState::DeleteValue(std::string key)
{
    stateTable.erase(key);
}