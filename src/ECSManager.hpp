#pragma once

#include <cassert>
#include <memory>
#include "./Entity.hpp"
#include "./components/Component.hpp"
#include "ObjectManager.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
namespace ECS
{
    static std::weak_ptr<Entity> CreateEntity()
    {
        return ObjectManager::GetInstance()->CreateObject<Entity>();
    }

    template <typename CT>
    static std::weak_ptr<CT> CreateComponent()
    {
        return ObjectManager::GetInstance()->CreateObject<CT>();
    }
}
