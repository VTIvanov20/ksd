#include "./Component.hpp"
#include "../util/graphics.hpp"

class BackgroundComponent : public Component
{
public:
    DEFINE_COMPONENT(BackgroundComponent);

    void OnCreate() override;
    void OnUpdate() override;
    void OnDestroy() override;

    void ChangeBGFile(std::string file);

    bool DestroyOnReload() override { return false; };

private:
    Texture bgTex;
};