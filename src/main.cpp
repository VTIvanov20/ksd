#include <iostream>
#include <memory>

#include "GameManager.hpp"


int main(int, char **)
{
    SetConfigFlags(FLAG_VSYNC_HINT);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    GameManager* gameManager = GameManager::GetInstance();
    gameManager->ChangeScene("res/scenes/main_menu.json");
    gameManager->Initialize();

    while (!gameManager->ShouldQuit())
    {
        gameManager->Update();
    }

    gameManager->Deinitialize();
    return 0;
}