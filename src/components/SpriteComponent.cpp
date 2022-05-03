#include "./SpriteComponent.hpp"
#include "./TransformComponent2D.hpp"

void SpriteComponent::OnCreate()
{
    texture = ObjectManager::GetInstance()->CreateObject<Texture>();
    texture.lock()->LoadFromFile(ImagePath.c_str());
}

void SpriteComponent::OnDestroy()
{
    if (!texture.expired())
        ObjectManager::GetInstance()->DestroyObjectFromID(texture.lock()->GetID());
}

void SpriteComponent::OnUpdate()
{
    if (!texture.expired())
    {
        std::shared_ptr<Texture> shTexture = texture.lock();
        std::shared_ptr<TransformComponent2D> transform = MGetComponent(TransformComponent2D);

        auto texRes = shTexture->GetSize();

        shTexture->Draw(transform->Position, transform->Scale,
            transform->Rotation, Vec2f { texRes.x / 2.f, texRes.y / 2.f });
    }
}

Vec2i SpriteComponent::GetSpriteRes()
{
    if (!texture.expired())
    {
        return texture.lock()->GetSize();
    } else return {};
}