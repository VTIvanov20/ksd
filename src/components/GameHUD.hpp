#include "./Component.hpp"

class GameHUD : public Component
{
public:
    /**
     * @brief Define a new component of type GameHUD
     */
    DEFINE_COMPONENT(GameHUD)
    ~GameHUD() = default;

    void OnUI() override;
};