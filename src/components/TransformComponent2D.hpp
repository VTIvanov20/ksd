#include "../util/la.hpp"
#include "./Component.hpp"

class TransformComponent2D : public Component
{
public:
    /**
     * @brief Defines a component of type TransformComponent2D
     */
    DEFINE_COMPONENT(TransformComponent2D)
    ~TransformComponent2D() = default;

    /**
     * @brief Position, rotation and scale properties of the component
     */
    Vec2f Position {};
    double Rotation {};
    Vec2f Scale {};
};