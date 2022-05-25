#include "./graphics.hpp"
#include "../GameManager.hpp"
#include "../config.h"
#include "./util/dear_imgui/backends/rlImGui.h"

bool Graphics::InitWindow(Vec2f winDimensions, const char* title)
{
    ::InitWindow(static_cast<int>(winDimensions.x), static_cast<int>(winDimensions.y), title);
    return true;
}

bool Graphics::CloseWindow()
{
    ::CloseWindow();
    return true;
}

Vec2i Graphics::GetWindowDimensions()
{
    return Vec2i {
        GetScreenWidth(),
        GetScreenHeight()
    };
}

void Graphics::BeginDrawing()
{
    if (WindowShouldClose())
        GameManager::GetInstance()->Quit();

    ::BeginDrawing();
    ::ClearBackground(BLACK);
}

void Graphics::EndDrawing()
{
    ::EndDrawing();
}

void Graphics::SetWindowPosition(Vec2f pos)
{
    ::SetWindowPosition(static_cast<int>(pos.x), static_cast<int>(pos.y));
}

// TODO!
void TextureObject::LoadEmpty(Vec2i res)
{
    renderTexture = LoadRenderTexture(res.x, res.y);
    verticallyMirroredTexture = LoadRenderTexture(res.x, res.y);
}

void TextureObject::LoadFromFile(const char *path)
{
    if (texture.id != 0) Unload();

    texture = LoadTexture(path);

    if (texture.id == 0)
    {
        printf("FATAL: Texture could not be loaded [%s]\n", path);
        exit(1);
    }
}

void TextureObject::Unload()
{
    UnloadTexture(texture);
    UnloadRenderTexture(renderTexture);
    UnloadRenderTexture(verticallyMirroredTexture);
}

void TextureObject::Draw(Recti Source, Rectf Destination, double Rotation, Vec2f Origin)
{
    if (renderTexture.id != 0)
        DrawTexturePro(renderTexture.texture,
            {
                -static_cast<float>(renderTexture.texture.width + Source.x),
                static_cast<float>(renderTexture.texture.height - Source.y),
                static_cast<float>(renderTexture.texture.width),
                -static_cast<float>(renderTexture.texture.height)
            },
            Destination, Origin, static_cast<float>(Rotation), WHITE);
    if (texture.id != 0)
        DrawTexturePro(texture, Source, Destination, Origin, static_cast<float>(Rotation), Color { 255, 255, 255, 255 });
}

void TextureObject::Draw(Vec2f Position, Vec2f Scale, double Rotation, Vec2f Origin)
{
    if (renderTexture.id != 0)
        DrawTexturePro(renderTexture.texture,
            {
                -static_cast<float>(renderTexture.texture.width),
                static_cast<float>(renderTexture.texture.height),
                static_cast<float>(renderTexture.texture.width),
                -static_cast<float>(renderTexture.texture.height)
            },
            {
                0, 0,
                static_cast<float>(renderTexture.texture.width) * Scale.x,
                static_cast<float>(renderTexture.texture.height) * Scale.y
            },
            Origin, static_cast<float>(Rotation), WHITE);
    if (texture.id != 0)
        DrawTexturePro(texture,
            { 0, 0, static_cast<float>(texture.width), static_cast<float>(texture.height) },
            { Position.x, Position.y, Scale.x * static_cast<float>(texture.width), Scale.y * static_cast<float>(texture.height) },
            Origin, static_cast<float>(Rotation), WHITE);
}

Vec2i TextureObject::GetSize()
{
    if (renderTexture.id != 0)
        return { renderTexture.texture.width, renderTexture.texture.height };
    if (texture.id != 0)
        return { texture.width, texture.height };
    return {};
}

TextureObject::~TextureObject()
{
    this->Unload();
}

void TextureObject::BeginDrawingTo()
{
    if (renderTexture.id == 0) return;
    BeginTextureMode(renderTexture);
}

void TextureObject::EndDrawingTo()
{
    if (renderTexture.id == 0) return;
    EndTextureMode();

    BeginTextureMode(verticallyMirroredTexture);
        DrawTexture(renderTexture.texture, 0, 0, WHITE);
    EndTextureMode();
}

Texture& TextureObject::GetTexture()
{
    return texture;
}

RenderTexture2D& TextureObject::GetRenderTexture()
{
    return verticallyMirroredTexture;
}

bool Input::IsKeyDown(int key)
{
    return ::IsKeyDown(key);
}

bool Input::IsKeyUp(int key)
{
    return ::IsKeyUp(key);
}

Vec2i Input::GetMousePos()
{
    Vec2f pos = GetMousePosition();
    return { static_cast<int>(pos.x), static_cast<int>(pos.y) };
}

Vec2i Input::GetMouseDelta()
{
    Vec2f delta = ::GetMouseDelta();
    return { static_cast<int>(delta.x), static_cast<int>(delta.y) };
}

void Input::SetMousePos(Vec2i pos)
{
    ::SetMousePosition(pos.x, pos.y);
}

bool Input::IsMouseButtonDown(int button)
{
    return ::IsMouseButtonDown(button);
}

bool Input::IsMouseButtonUp(int button)
{
    return ::IsMouseButtonUp(button);
}

bool RLImGui::InitImGui()
{
    IMGUI_CHECKVERSION();

    SetupRLImGui(true);

    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.Fonts->AddFontFromFileTTF("res/fonts/Ubuntu_Mono/UbuntuMono-Bold.ttf", 15);
    ReloadImGuiFonts();

    return true;
}

void RLImGui::CloseImGui()
{
    ShutdownRLImGui();
}

void RLImGui::BeginImGuiDrawing()
{
    BeginRLImGui();
}

void RLImGui::EndImGuiDrawing()
{
    EndRLImGui();
}