#include "GameManager.hpp"
#include "ECSManager.hpp"

GameManager* GameManager::instance = nullptr;

GameManager* GameManager::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new GameManager();
    }

    return instance;
}

void GameManager::ReinitializeScene()
{
    // Destroy any leftover drawables to prevent memory leaking
    ObjectManager::GetInstance()->DestroyAllEntities();
    InitSceneFromFile(state.scenePath);
    ObjectManager::GetInstance()->TriggerCreateEvents();
    
    state.scene_update = false;
}

void GameManager::Initialize()
{
    // Initializes the windows
    if(!Graphics::InitWindow(Vec2f{ WINDOW_WIDTH, WINDOW_HEIGHT }, WINDOW_TITLE,
        static_cast<SDL_WindowFlags>(0), static_cast<SDL_RendererFlags>(0))) /* a very annoying fpermissive warning */
        exit(1);

    if(!SDLImGui::InitImGui())
        exit(1);

    ReinitializeScene();
}

void GameManager::Quit()
{
    // Sets the bool to true
    std::cout << "INFO: GameManager: Triggered should_quit boolean" << std::endl;
    state.should_quit = true;
}

void GameManager::Update()
{
    // If a scene has recently been changes initialize it
    if (state.scene_update)
    {
        ReinitializeScene();
        std::cout << "INFO: GameManager: Scene Update!" << std::endl;
    }

    Graphics::BeginDrawing();

    // Draws and updates all drawables
    ObjectManager::GetInstance()->TriggerUpdateEvents();

    Graphics::EndDrawing();
}

void GameManager::Deinitialize()
{
    // Destroy any leftover drawables to prevent memory leaking
    ObjectManager::GetInstance()->DestroyAllEntities();

    SDLImGui::CloseImGui();
    Graphics::CloseWindow();

    std::cout << "INFO: Goodbye!" << std::endl;
}

bool GameManager::ShouldQuit()
{
    // Returns if the window should quit
    return state.should_quit;
}

void GameManager::ChangeScene(std::string scenePath)
{
    state.scenePath = scenePath;
    state.scene_update = true;
}