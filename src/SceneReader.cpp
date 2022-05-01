#include "SceneReader.hpp"
#include "ECSManager.hpp"
#include "./util/json.hpp"

#include <fstream>
#include <cassert>

#include "./components/MainMenu.hpp"

std::string GetFileContents(const std::string path)
{
    std::stringstream jsonStream;

    std::ifstream fJson;

    fJson.open(path, std::ios::in);

    if (!fJson.is_open())
        throw std::runtime_error("File \"" + path + "\" could not be opened");
    
    std::string jsonLine;
    while (std::getline(fJson, jsonLine))
        jsonStream << jsonLine << '\n';
    
    fJson.close();

    return jsonStream.str();
}

void InitSceneFromFile(const std::string fPath)
{
    std::string contents = GetFileContents(fPath);
    nlohmann::json sJson = nlohmann::json::parse(contents);

    assert(sJson.is_array() && "Invalid Scene JSON File!");

    for (auto ent : sJson)
    {
        auto entity = ECS::CreateEntity();

        for (auto [name, component] : ent["components"].items())
        {
            if (name == "MainMenu")
                entity.lock()->BindComponent(ECS::CreateComponent<MainMenu>());
        }
    }
}