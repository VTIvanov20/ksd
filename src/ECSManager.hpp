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
    /**
     * @brief Creates a new entity and returns it
     */
    static std::weak_ptr<Entity> CreateEntity()
    {
        return ObjectManager::GetInstance()->CreateObject<Entity>();
    }

    /**
     * @brief Creates a new component of the given type and returns it
     */
    template <typename CT>
    static std::weak_ptr<CT> CreateComponent()
    {
        return ObjectManager::GetInstance()->CreateObject<CT>();
    }
}
