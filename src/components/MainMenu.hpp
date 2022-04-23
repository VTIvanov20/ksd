#pragma once

#include "UIComponent.hpp"

class MainMenu : public UIComponent
{
public:
    DEFINE_DERIVED_COMPONENT(MainMenu, UIComponent)
    void OnUI() override;
};