#include "MainMenu.hpp"
#include <iostream>

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
    multiplayerCodeBuf = new char[256];
}
void MainMenu::OnDestroy()
{
    delete[] multiplayerCodeBuf;
}

void MainMenu::OnUpdate()
{
    
}