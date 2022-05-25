#include "./Component.hpp"
#include "../util/graphics.hpp"

#include "./CardTypes.hpp"
#include "./GameController.hpp"

#include <map>

class GameWindow : public Component
{
public:
    DEFINE_COMPONENT(GameWindow)

    void OnCreate() override;
    void OnUpdate() override;
    void OnDestroy() override;
    void OnUI() override;
    bool DrawImageButton(CardType type);

private:
    void DrawBeginningNode(BeginningNode<CardType> top, Vec2f idx);
    void DrawNode(Node<CardType> top, Vec2f idx, VerticalCardPos vertPos);
    void DrawFromType(CardType type, Vec2f index, VerticalCardPos vertPos);

    std::unordered_map<CardType, std::weak_ptr<TextureObject>> cardTextures;

    std::weak_ptr<TextureObject> renderTexture;
    std::vector<Vec2i> clickablePositions {};

    const float narrowness = .75f;
    const Vec2f scaleTo { 142.f / 2.f, 204.f / 2.f };
};