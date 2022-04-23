#pragma once

#include "./Component.hpp"
#include "../util/graphics.hpp"

class UIComponent : public Component
{
public:
    DEFINE_COMPONENT(UIComponent)

    void OnUpdate() override;

    virtual void OnUI() {};
};