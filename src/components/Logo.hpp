#pragma once

#include "./Component.hpp"
#include "../util/graphics.hpp"

class Logo : public Component
{
public:
    DEFINE_COMPONENT(Logo)
    virtual ~Logo() = default;

    void OnCreate() override;
    void OnUpdate() override;
    void OnDestroy() override;

    std::string LogoPath = "res/img/logo.png";

private:
    std::weak_ptr<TextureObject> logoTex;
};