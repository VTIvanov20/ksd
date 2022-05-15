#include "./Component.hpp"
#include "../util/graphics.hpp"

class BackgroundComponent : public Component
{
public:
    /**
     * @brief Define a new component of type BackgroundComponent
     */
    DEFINE_COMPONENT(BackgroundComponent);

    void OnCreate() override;
    void OnUpdate() override;
    void OnDestroy() override;

    /**
     * @brief Change the background file
     * 
     * @param file The path to the background file
     */
    void ChangeBGFile(std::string file);

    bool DestroyOnReload() override { return false; };

private:
    /**
     * @brief The texture for the background 
     */
    Texture bgTex;
};