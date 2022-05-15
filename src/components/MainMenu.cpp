#include "MainMenu.hpp"
#include <iostream>
#include "./GlobalState.hpp"
#include "./NetworkController.hpp"

/* - Main Menu Layout -

    MainMenuState::MAIN_MENU
        - Singleplayer -> MainMenuState::SINGLEPLAYER
        - Multiplayer -> MainMenuState::MULTIPLAYER
        - Quit -> GameManager::GetInstance()->Quit()

    MainMenuState::SINGLEPLAYER
        - Without NOT card -> Init game
        - With NOT card -> Init game
        - Back -> MainMenuState::MAIN_MENU

    MainMenuState::MULTIPLAYER_HOST
        - Without NOT card -> Init game
        - With NOT card -> Init game
        - Back -> MainMenuState::MAIN_MENU

    MainMenuState::MULTIPLAYER_JOIN
        - Textbox
        - Join -> Init multiplayer game
        - Back -> MainMenuState::MULTIPLAYER

*/

void MainMenu::OnCreate()
{
    multiplayerCodeBuf = new char[16];
    multiplayerCodeBuf[0] = '\0';
}

MainMenu::~MainMenu()
{
    delete[] multiplayerCodeBuf;
}

void MainMenu::OnUI()
{
    auto networkControllerEntity =
        std::static_pointer_cast<Entity>(
            ObjectManager::GetInstance()->GetEntityFromTagName("network_controller").lock());
    auto networkController = MGetComponentFrom(networkControllerEntity, NetworkController);
    
    const ImVec2 displaySize = ImGui::GetIO().DisplaySize;
    const ImVec2 windowPadding = ImGui::GetStyle().WindowPadding;
    const ImVec2 windowPos {
        displaySize.x / 2 - windowSize.x / 2,
        displaySize.y / 2 - windowSize.y / 2
    };

    const ImVec2 buttonSize {
        -1,
        windowSize.y / 3 - windowPadding.y
    };

    ImGui::SetNextWindowSize(windowSize);
    ImGui::SetNextWindowPos(windowPos);
    ImGui::Begin("Main Menu", nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking);
    switch(state)
    {
        case MainMenuState::MAIN_MENU:
        {
            if(ImGui::Button("Singleplayer", buttonSize))
            {
                state = MainMenuState::SINGLEPLAYER;
            }
            if(ImGui::Button("Multiplayer", buttonSize))
            {
                state = MainMenuState::MULTIPLAYER;
            }
            if(ImGui::Button("Quit", buttonSize))
            {
                GameManager::GetInstance()->Quit();
            }
            break;
        }
        case MainMenuState::SINGLEPLAYER:
        {
            if(ImGui::Button("Without NOT card", buttonSize))
            {
                auto globalState = std::static_pointer_cast<Entity>(
                    ObjectManager::GetInstance()->GetEntityFromTagName("global_state").lock());
                MGetComponentFrom(globalState, GlobalState)->SetValue("mode", "singleplayer");
                GameManager::GetInstance()->ChangeScene("res/scenes/game_scene.json");
            }
            if(ImGui::Button("With NOT card", buttonSize))
            {
                // GameManager::GetInstance()->ChangeScene("res/scenes/game_scene.json");
            }
            if(ImGui::Button("Back", buttonSize))
            {
                state = MainMenuState::MAIN_MENU;
            }
            break;
        }
        case MainMenuState::MULTIPLAYER:
        {
            if(ImGui::Button("Host", buttonSize))
            {
                state = MainMenuState::MULTIPLAYER_HOST;
            }
            if(ImGui::Button("Join", buttonSize))
            {
                state = MainMenuState::MULTIPLAYER_JOIN;
            }
            if(ImGui::Button("Back", buttonSize))
            {
                state = MainMenuState::MAIN_MENU;
            }
            break;
        }
        case MainMenuState::MULTIPLAYER_HOST:
        {
            if(ImGui::Button("Without NOT card", buttonSize))
            {
                auto globalState = std::static_pointer_cast<Entity>(
                    ObjectManager::GetInstance()->GetEntityFromTagName("global_state").lock());
                networkController->CreateRoom();
                MGetComponentFrom(globalState, GlobalState)->SetValue("mode", "multiplayer");

                state = MainMenuState::WAITING_FOR_PLAYERS_MULTIPLAYER;

                // GameManager::GetInstance()->ChangeScene("res/scenes/game_scene.json");
            }
            if(ImGui::Button("With NOT card", buttonSize))
            {
                // GameManager::GetInstance()->ChangeScene("res/scenes/game_scene.json");
            }
            if(ImGui::Button("Back", buttonSize))
            {
                state = MainMenuState::MULTIPLAYER;
            }
            break;
        }
        case MainMenuState::MULTIPLAYER_JOIN:
        {
            ImGui::Text("Enter game code:");
            if(ImGui::InputText(" ", multiplayerCodeBuf, 9))
            {
                // GameManager::GetInstance()->JoinMultiplayerGame();
            }
            ImGui::SameLine();
            if(ImGui::Button("Join", ImVec2(100, 0)))
            {
                auto globalState = std::static_pointer_cast<Entity>(
                    ObjectManager::GetInstance()->GetEntityFromTagName("global_state").lock());
                networkController->JoinRoom(std::string { multiplayerCodeBuf });
                MGetComponentFrom(globalState, GlobalState)->SetValue("mode", "multiplayer");

                state = MainMenuState::WAITING_FOR_PLAYERS_MULTIPLAYER;
                // GameManager::GetInstance()->JoinMultiplayerGame();
            }
            if(ImGui::Button("Back"))
            {
                state = MainMenuState::MULTIPLAYER;
            }
            break;
        }
        case MainMenuState::WAITING_FOR_PLAYERS_MULTIPLAYER:
        {
            auto nState = networkController->GetState();
            ImGui::Text("Waiting for a player to join...");
            ImGui::Text("inGame = %s", nState.inGame ? "true" : "false");
            ImGui::Text("gameStarted = %s", nState.gameStarted ? "true" : "false");
            ImGui::Text("code = %s", nState.code.c_str());

            if (nState.gameStarted)
                GameManager::GetInstance()->ChangeScene("res/scenes/game_scene.json");
        }
    }
    ImGui::End();
}