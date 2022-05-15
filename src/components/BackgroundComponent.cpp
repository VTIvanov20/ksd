#include "./BackgroundComponent.hpp"

void BackgroundComponent::OnCreate()
{
    // tex = ObjectManager::GetInstance()->CreateObject<TextureObject>();
    // bgTex.lock()->LoadFromFile("res/img/themes/wildwest/bg.png");
    bgTex = LoadTexture("res/img/themes/wildwest/bg.png");
}

void BackgroundComponent::OnUpdate()
{
    DrawTexturePro(bgTex, {
        0, 0,
        (float)bgTex.width, (float)bgTex.height
    }, {
        0.f, 0.f,
        (float)GetScreenWidth(),
        (float)GetScreenHeight()
    }, {0.f, 0.f}, 0.f, { 255, 255, 255, 191 });
}

void BackgroundComponent::OnDestroy()
{
    UnloadTexture(bgTex);
}

void BackgroundComponent::ChangeBGFile(std::string file)
{
    UnloadTexture(bgTex);
    bgTex = LoadTexture(file.c_str());
}