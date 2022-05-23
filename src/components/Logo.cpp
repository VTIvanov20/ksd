#include "./Logo.hpp"

void Logo::OnCreate()
{
    logoTex = ObjectManager::GetInstance()->CreateObject<TextureObject>();
    logoTex.lock()->LoadFromFile(LogoPath.c_str());
}

void Logo::OnUpdate()
{
    const ImVec2 displaySize = ImGui::GetIO().DisplaySize;
    auto logoTexRes = logoTex.lock()->GetSize();
    logoTex.lock()->Draw({ displaySize.x / 2, displaySize.y / 4 }, { 1.f, 1.f }, 0.f, { logoTexRes.x / 2.f, logoTexRes.y / 2.f });
}

void Logo::OnDestroy()
{
    logoTex.lock()->Unload();
    ObjectManager::GetInstance()->DestroyObjectFromID(logoTex.lock()->GetID());
}