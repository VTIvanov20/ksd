#include "MainMenu.hpp"
#include <iostream>

void MainMenu::OnCreate()
{
    textBuf = new char[256];
}
void MainMenu::OnDestroy()
{
    delete[] textBuf;
}

void MainMenu::OnUI()
{
    ImGui::Begin("Main Menu");
    switch(state)
    {
        case MainMenuState::MAIN_MENU:
            if(ImGui::Button("Singleplayer"))
            {
                state = MainMenuState::SINGLEPLAYER;
            }
            if(ImGui::Button("Multiplayer"))
            {
                state = MainMenuState::MULTIPLAYER;
            }
            if(ImGui::Button("Quit"))
            {
                GameManager::GetInstance()->Quit();
            }
            break;
        case MainMenuState::SINGLEPLAYER:
            if(ImGui::Button("Without NOT card"))
            {
                // GameManager::GetInstance()->StartSinglePlayerGame();
            }
            if(ImGui::Button("With NOT card"))
            {
                // GameManager::GetInstance()->StartSinglePlayerGame();
            }
            if(ImGui::Button("Back"))
            {
                state = MainMenuState::MAIN_MENU;
            }
            break;
        case MainMenuState::MULTIPLAYER:
            if(ImGui::Button("Host"))
            {
                state = MainMenuState::MULTIPLAYER_HOST;
                // GameManager::GetInstance()->StartMultiplayerGame();
            }
            if(ImGui::Button("Join"))
            {
                state = MainMenuState::MULTIPLAYER_JOIN;
                // GameManager::GetInstance()->JoinMultiplayerGame();
            }
            if(ImGui::Button("Back"))
            {
                state = MainMenuState::MAIN_MENU;
            }
            break;
        case MainMenuState::MULTIPLAYER_HOST:
            if(ImGui::Button("Without NOT card"))
            {
                // GameManager::GetInstance()->StartMultiplayerGame();
            }
            if(ImGui::Button("With NOT card"))
            {
                // GameManager::GetInstance()->StartMultiplayerGame();
            }
            if(ImGui::Button("Back"))
            {
                state = MainMenuState::MULTIPLAYER;
            }
            break;
        case MainMenuState::MULTIPLAYER_JOIN:
            if(ImGui::InputText("Enter game code", textBuf, 9))
            {
                // GameManager::GetInstance()->JoinMultiplayerGame();
            }
            if(ImGui::Button("Back"))
            {
                state = MainMenuState::MULTIPLAYER;
            }
            break;
    }
    // if(ImGui::Button("Singleplayer")) {
    //     std::cout << "Singleplayer\n";
    // }
    // if(ImGui::Button("Multiplayer")) {
    //     std::cout << "Multiplayer!\n";
    // }
    ImGui::End();
}