#include "SceneReader.hpp"
#include "ECSManager.hpp"
#include "./util/json.hpp"

#include <fstream>
#include <cassert>

#include "./components/MainMenu.hpp"
#include "./components/TransformComponent2D.hpp"
#include "./components/SpriteComponent.hpp"
#include "./components/GlobalState.hpp"
#include "./components/GameController.hpp"
#include "./components/GameWindow.hpp"
#include "./components/GameHUD.hpp"
#include "./components/NetworkController.hpp"

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
        std::weak_ptr<Entity> entity;
        
        try {
            std::string tag;
            ent["TagName"].get_to(tag);

            if (ObjectManager::GetInstance()->TagNameAlreadyExists(tag))
                continue;

            entity = ECS::CreateEntity();
            entity.lock()->TagName = tag;
        } catch(const nlohmann::json::exception&) {
            printf("WARN: TagName is possibly missing in an entity\n");
            entity = ECS::CreateEntity();
        }

        try {
            ent["DestroyOnReload"].get_to(entity.lock()->_destroyOnReload);
        } catch(const std::exception& e) {}

        for (auto [name, component] : ent["components"].items())
        {
            if (name == "MainMenu")
                entity.lock()->BindComponent(ECS::CreateComponent<MainMenu>());
            else if (name == "TransformComponent2D")
            {
                auto cmp = ECS::CreateComponent<TransformComponent2D>().lock();

                component["Position"]["x"].get_to(cmp->Position.x);
                component["Position"]["y"].get_to(cmp->Position.y);

                component["Rotation"].get_to(cmp->Rotation);

                component["Scale"]["x"].get_to(cmp->Scale.x);
                component["Scale"]["y"].get_to(cmp->Scale.y);

                entity.lock()->BindComponent(cmp);
            }
            else if (name == "SpriteComponent")
            {
                auto cmp = ECS::CreateComponent<SpriteComponent>().lock();

                component["ImagePath"].get_to(cmp->ImagePath);

                entity.lock()->BindComponent(cmp);
            }
            // else if (name == "BehaviouralComponent")
            // {
            //     auto cmp = ECS::CreateComponent<BehaviouralComponent>().lock();

            //     component["Velocity"].get_to(cmp->Velocity);

            //     entity.lock()->BindComponent(cmp);
            // }
            else if (name == "GlobalState")
            {
                entity.lock()->BindComponent(ECS::CreateComponent<GlobalState>().lock());
            }
            else if (name == "GameController")
            {
                entity.lock()->BindComponent(ECS::CreateComponent<GameController>().lock());
            }
            else if (name == "GameWindow")
            {
                entity.lock()->BindComponent(ECS::CreateComponent<GameWindow>().lock());
            }
            else if (name == "GameHUD")
            {
                entity.lock()->BindComponent(ECS::CreateComponent<GameHUD>().lock());
            }
            else if (name == "NetworkController")
            {
                auto cmp = ECS::CreateComponent<NetworkController>().lock();

                component["WebSocketURI"].get_to(cmp->WsUri);

                entity.lock()->BindComponent(cmp);
            }
        }
    }
}