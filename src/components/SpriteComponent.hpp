#include "./Component.hpp"
#include "../util/graphics.hpp"

class SpriteComponent : public Component
{
public:
    DEFINE_COMPONENT(SpriteComponent)
    ~SpriteComponent() = default;

    void OnCreate() override;
    void OnUpdate() override;
    void OnDestroy() override;

    Vec2i GetSpriteRes();

    std::string ImagePath;

private:
    std::weak_ptr<Texture> texture;
};