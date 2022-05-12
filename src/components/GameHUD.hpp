#include "./Component.hpp"

class GameHUD : public Component
{
public:
    DEFINE_COMPONENT(GameHUD);
    ~GameHUD() = default;

    void OnUI() override;
};