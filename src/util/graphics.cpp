#include "./graphics.hpp"
#include "../GameManager.hpp"
#include "../config.h"

// suggestion: maybe this library could open more than one window in the future??

static SDL_Window *winHandle = nullptr;
static SDL_Renderer *winRenderer = nullptr;
static std::map<SDL_Keycode, uint8_t> keyStates;
static std::map<uint8_t, uint8_t> mouseBtnStates;

bool imguiInitialized = false;

bool Graphics::InitWindow(
    Vec2f winDimensions, const char* title,
    SDL_WindowFlags winFlags, SDL_RendererFlags rendFlags)
{
    if (winFlags == 0)
        winFlags = static_cast<SDL_WindowFlags>(SDL_WINDOW_FLAGS);

    if (rendFlags == 0)
        rendFlags = static_cast<SDL_RendererFlags>(SDL_RENDERER_FLAGS);
    
    if (winHandle != nullptr)
    {
        printf("WARN: Window has already been initialized\n");
        return false;
    }

    if (SDL_WasInit(SDL_INIT_VIDEO) == 0)
    {
        printf("WARN: SDL probably wasn't initialized properly!\n");
        SDL_Init(SDL_INIT_VIDEO);
    }

    SDL_DisplayMode dm;

    // get information about the first screen
    SDL_GetCurrentDisplayMode(0, &dm);

    Vec2f winPos {
        dm.w / 2 - winDimensions.x / 2,
        dm.h / 2 - winDimensions.y / 2
    };

    bool success = false;
    
    winHandle = SDL_CreateWindow(
        title, winPos.x, winPos.y,
        winDimensions.x, winDimensions.y,
        winFlags);

    if (winHandle == nullptr)
    {
        printf("ERROR: Could not create window: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        printf("INFO: Successfully initalized window!\n");
        printf("INFO: Window Dimensions: %.0f, %.0f\n", winDimensions.x, winDimensions.y);
        printf("INFO: Screen Dimensions: %d, %d\n", dm.w, dm.h);
        success = true;
    }

    winRenderer = SDL_CreateRenderer(winHandle, -1, rendFlags);
    
    if (winRenderer == nullptr)
    {
        printf("ERROR: Could not create a renderer: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        printf("INFO: Successfully initialized renderer!\n");
        success = true;

        SDL_RendererInfo rendInfo{};
        if(SDL_GetRendererInfo(winRenderer, &rendInfo) != 0)
            printf("WARN: Could not get information about the renderer: %s\n", SDL_GetError());
        else
        {
            printf("INFO: Renderer Name: %s\n", rendInfo.name);
            printf("INFO: Max texture height: %d\n", rendInfo.max_texture_height);
            printf("INFO: Max texture width: %d\n", rendInfo.max_texture_width);
        }
    }

    return success;
}

bool Graphics::CloseWindow()
{
    if (winHandle == nullptr)
    {
        printf("WARN: Tried to destroy a window that was not opened\n");
        return false;
    }
    else
    {
        SDL_DestroyWindow(winHandle);
        SDL_DestroyRenderer(winRenderer);
        
        printf("INFO: Successfully closed window!\n");
        winHandle = nullptr;
        winRenderer = nullptr;
        return true;
    }
}

Vec2i Graphics::GetWindowDimensions()
{
    if (winHandle != nullptr)
    {
        int w, h;

        SDL_GetWindowSize(winHandle, &w, &h);

        return Vec2i { w, h };
    } else return Vec2i {};
}

void Graphics::BeginDrawing()
{
    if (winHandle == nullptr)
    {
        printf("WARN: Window must be initialized before calling BeginDrawing");
        return;
    }

    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (imguiInitialized)
            ImGui_ImplSDL2_ProcessEvent(&e);
        
        switch(e.type)
        {
        case SDL_QUIT:
            GameManager::GetInstance()->Quit();
            break;
        case SDL_KEYUP:
        case SDL_KEYDOWN:
            keyStates[e.key.keysym.sym] = e.key.state;
            break;
        case SDL_MOUSEBUTTONUP:
        case SDL_MOUSEBUTTONDOWN:
            mouseBtnStates[e.button.button] = e.button.state;
            break;
        }
    }

    SDL_RenderClear(winRenderer);
}

void Graphics::EndDrawing()
{
    if (winHandle == nullptr)
    {
        printf("WARN: Window must be initialized before calling EndDrawing");
        return;
    }

    SDL_RenderPresent(winRenderer);
}

void Graphics::SetWindowPosition(Vec2f pos)
{
    if (winHandle == nullptr)
    {
        printf("ERROR: No main window initalized to change its position\n");
        return;
    }

    SDL_SetWindowPosition(winHandle, (int)pos.x, (int)pos.y);
}

void Texture::LoadTexture(const char *path)
{
    if (winHandle == nullptr || winRenderer == nullptr)
    {
        printf("ERROR: Cannot create texture without a renderer\n");
    }
    
    SDL_Texture *texture = IMG_LoadTexture(winRenderer, path);

    if (texture == nullptr)
    {
        printf("FATAL: Couldn't load image \"%s\"\n", path);
        printf("FATAL: Error message: %s\n", SDL_GetError());
        exit(1);
    }

    this->texture = texture;
}

void Texture::UnloadTexture()
{
    if (this->texture != nullptr)
    {
        SDL_DestroyTexture(this->texture);
        this->texture = nullptr;
    }
}

void Texture::DrawTexture()
{
    if (this->texture != nullptr)
    {
        SDL_RenderCopy(winRenderer, this->texture, 
            &Source, &Destination);
    }
}

Vec2i Texture::GetSize()
{
    Vec2i out{};
    if (this->texture != nullptr)
        SDL_QueryTexture(this->texture, NULL, NULL, &out.x, &out.y);
    
    return out;
}

Texture::~Texture()
{
    this->UnloadTexture();
}

bool Input::IsKeyDown(SDL_KeyCode code)
{
    if (keyStates.find(code) != keyStates.end())
        return keyStates[code];
    else return false;
}

bool Input::IsKeyUp(SDL_KeyCode code)
{
    return !IsKeyDown(code);
}

Vec2i Input::GetMousePos()
{
    if (winHandle == nullptr)
        return Vec2i {};
    
    int x, y;

    SDL_PumpEvents();
    SDL_GetMouseState(&x, &y);

    return Vec2i { x, y };
}

Vec2i Input::GetMouseDelta()
{
    if (winHandle == nullptr)
        return Vec2i {};

    int x, y;

    SDL_PumpEvents();
    SDL_GetRelativeMouseState(&x, &y);

    return Vec2i { x, y };
}

void Input::SetMousePos(Vec2i pos)
{
    if (winHandle != nullptr)
        SDL_WarpMouseInWindow(winHandle, pos.x, pos.y);
}

bool Input::IsMouseButtonDown(uint8_t button)
{
    if (mouseBtnStates.find(button) != mouseBtnStates.end())
        return mouseBtnStates[button];
    else return false;
}

bool Input::IsMouseButtonUp(uint8_t button)
{
    return !IsMouseButtonDown(button);
}

bool SDLImGui::InitImGui()
{
    bool success = true;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    success = ImGui_ImplSDL2_InitForSDLRenderer(winHandle, winRenderer);
    success = ImGui_ImplSDLRenderer_Init(winRenderer);

    if (success) imguiInitialized = true;

    return success;
}

void SDLImGui::CloseImGui()
{
    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();

    imguiInitialized = false;
}

void SDLImGui::BeginImGuiDrawing()
{
    ImGui_ImplSDLRenderer_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void SDLImGui::EndImGuiDrawing()
{
    ImGui::Render();
    ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
}