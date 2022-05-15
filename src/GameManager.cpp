#include "GameManager.hpp"
#include "ECSManager.hpp"

/**
* @brief Creates a null pointer instance of the singleton class GameManager
*/
GameManager* GameManager::instance = nullptr;

GameManager* GameManager::GetInstance()
{
    /**
     * @brief If an instance of the singleton doesn't exist, creates one
     */
    if (instance == nullptr)
    {
        instance = new GameManager();
    }

    return instance;
}

void GameManager::ReinitializeScene()
{
    /**
     * @brief Destroys any leftover drawables to prevent memory leaking
     */
    ObjectManager::GetInstance()->DestroyAllEntities();
    InitSceneFromFile(state.scenePath);
    ObjectManager::GetInstance()->TriggerCreateEvents();
    
    /**
     * @brief After the scene is updated, stops it from updating again
     */
    state.scene_update = false;
}

void GameManager::Initialize()
{
    /**
     * @brief Initializes the window and imgui
     */
    if(!Graphics::InitWindow(Vec2f{ WINDOW_WIDTH, WINDOW_HEIGHT }, WINDOW_TITLE))
        exit(1);

    if(!RLImGui::InitImGui())
        exit(1);

    ReinitializeScene();
}

void GameManager::Quit()
{
    /**
     * @brief Makes the game quit
     */
    std::cout << "INFO: GameManager: Triggered should_quit boolean" << std::endl;
    state.should_quit = true;
}

void GameManager::Update()
{
    /**
     * @brief If a scene has been changed, updates it
     */
    if (state.scene_update)
    {
        ReinitializeScene();
        std::cout << "INFO: GameManager: Scene Update!" << std::endl;
    }

    Graphics::BeginDrawing();

    /**
     * @brief  Draws and updates all drawables
     */
    ObjectManager::GetInstance()->TriggerUpdateEvents();

    Graphics::EndDrawing();
}

void GameManager::Deinitialize()
{
    /**
     * @brief Destroys any leftover drawables to prevent memory leaking
     */
    ObjectManager::GetInstance()->DestroyAllEntities(false);

    RLImGui::CloseImGui();
    Graphics::CloseWindow();

    std::cout << "INFO: Goodbye!" << std::endl;
}

bool GameManager::ShouldQuit()
{
    /**
     * @brief Returns if the window should quit
     */
    return state.should_quit;
}

void GameManager::ChangeScene(std::string scenePath)
{
    /**
     * @brief Sets the path for the scene and makes it update
     * 
     * @param scenePath Gets the path of the .json file for the scene
     */
    state.scenePath = scenePath;
    state.scene_update = true;
}