#include "../util/la.hpp"
#include "./Component.hpp"

class TransformComponent2D : public Component
{
public:
    DEFINE_COMPONENT(TransformComponent2D)
    ~TransformComponent2D() = default;

    Vec2f Position {};
    double Rotation {};
    Vec2f Scale {};
};