#include "./SpriteComponent.hpp"
#include "./TransformComponent2D.hpp"

void SpriteComponent::OnCreate()
{
    /**
     * @brief Creates object and loads the image for the sprite when it's created
     */
    texture = ObjectManager::GetInstance()->CreateObject<TextureObject>();
    texture.lock()->LoadFromFile(ImagePath.c_str());
}

void SpriteComponent::OnDestroy()
{
    /**
     * @brief If the texture isn't expired, destroy it
     */
    if (!texture.expired())
        ObjectManager::GetInstance()->DestroyObjectFromID(texture.lock()->GetID());
}

void SpriteComponent::OnUpdate()
{
    /**
     * @brief If the texture isn't expired, get its size and draw it
     */
    if (!texture.expired())
    {
        std::shared_ptr<TextureObject> shTexture = texture.lock();
        std::shared_ptr<TransformComponent2D> transform = MGetComponent(TransformComponent2D);

        auto texRes = shTexture->GetSize();

        shTexture->Draw(transform->Position, transform->Scale,
            transform->Rotation, Vec2f { texRes.x / 2.f, texRes.y / 2.f });
    }
}

Vec2i SpriteComponent::GetSpriteRes()
{
    /**
     * @brief Get the size of the texture and return it
     */
    if (!texture.expired())
    {
        return texture.lock()->GetSize();
    } else return {};
}