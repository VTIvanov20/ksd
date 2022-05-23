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
#include "./components/BackgroundComponent.hpp"
#include "./components/Logo.hpp"

/**
 * @brief Gets the contents of the .json file
 * 
 * @param path The path to the .json file
 * 
 * @return Returns a string with all the components on an entity
 */
std::string GetFileContents(const std::string path)
{
    std::stringstream jsonStream;

    std::ifstream fJson;

    /**
     * @brief Open the file specified in the path param
     */
    fJson.open(path, std::ios::in);

    if (!fJson.is_open())
        throw std::runtime_error("File \"" + path + "\" could not be opened");
    
    /**
     * @brief While there is still information on the file, extract it. When there is no more information, close the file
     */
    std::string jsonLine;
    while (std::getline(fJson, jsonLine))
        jsonStream << jsonLine << '\n';
    
    fJson.close();

    /**
     * @brief Return the stringstream with the information on the file
     */
    return jsonStream.str();
}

void InitSceneFromFile(const std::string fPath)
{
    /**
     * @brief Gets the contents of the file in the fPath param with the GetFileContents() function
     */
    std::string contents = GetFileContents(fPath);
    nlohmann::json sJson = nlohmann::json::parse(contents);

    assert(sJson.is_array() && "Invalid Scene JSON File!");

    /**
     * @brief Create an entity for every entity in the .json file, and give it its tag name if it doesn't already exist
     */
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

        /**
         * @brief For every component in the just created entity, create it and bind it to it
         */
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
            else if (name == "BackgroundComponent")
            {
                entity.lock()->BindComponent(ECS::CreateComponent<BackgroundComponent>());
            }
            else if (name == "Logo")
            {
                auto cmp = ECS::CreateComponent<Logo>().lock();

                component["LogoPath"].get_to(cmp->LogoPath);

                entity.lock()->BindComponent(cmp);
            }
        }
    }
}