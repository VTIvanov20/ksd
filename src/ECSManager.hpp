#include <cassert>
#include <memory>
#include "./Entity.hpp"
#include "./components/Component.hpp"
#include "ObjectManager.hpp"

namespace ECS
{
    std::weak_ptr<Entity> CreateEntity()
    {
        return ObjectManager::GetInstance()->CreateObject<Entity>();
    }

    template <typename CT>
    std::weak_ptr<CT> CreateComponent()
    {
        return ObjectManager::GetInstance()->CreateObject<CT>();
    }
}
