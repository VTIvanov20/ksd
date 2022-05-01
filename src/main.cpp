#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <memory>

#include "GameManager.hpp"

int main(int, char **)
{
    SDL_Init(SDL_INIT_FLAGS);
    IMG_Init(IMG_INIT_FLAGS);

    GameManager* gameManager = GameManager::GetInstance();
    gameManager->ChangeScene("res/scenes/main_menu.json");
    gameManager->Initialize();

    while (!gameManager->ShouldQuit())
    {
        gameManager->Update();
    }

    gameManager->Deinitialize();

    SDL_Quit();
    IMG_Quit();
    return 0;
}