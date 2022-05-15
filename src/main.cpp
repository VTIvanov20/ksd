#include <iostream>
#include <memory>

#include "GameManager.hpp"


int main(int, char **)
{
    /**
     * @brief Sets the config flags for the window
     */
    SetConfigFlags(FLAG_VSYNC_HINT);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetConfigFlags(FLAG_MSAA_4X_HINT);

    /**
     * @brief Creates a GameManager instance, changes the scene to the main menu and then initializes the window
     */
    GameManager* gameManager = GameManager::GetInstance();
    gameManager->ChangeScene("res/scenes/main_menu.json");
    gameManager->Initialize();
    

    /**
     * @brief Sets the exit key to 0, so the window doesn't quiz except we want it to
     */
    SetExitKey(0);

    /**
     * @brief The main game loop. The game updates while it hasn't quit and once it has it deinitializes the window
     */
    while (!gameManager->ShouldQuit())
    {
        gameManager->Update();
    }

    gameManager->Deinitialize();
    return 0;
}