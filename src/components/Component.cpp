#include "./Component.hpp"
#include "../Entity.hpp"

void Component::BindToEntity(std::weak_ptr<Object> object)
{
    assert(!object.expired() && "Passed an expired pointer");
    assert(ObjectManager::CheckBaseName(object, "Entity") && "The passed object was not an Entity");
    IsUsed = true;
    boundEntity = object;
}

void Component::BindToEntity(uint64_t id)
{
    auto object = ObjectManager::GetInstance()->GetObjectFromID(id);
    BindToEntity(object);
}

std::weak_ptr<Component> Component::GetComponent(std::string componentName)
{
    auto entity = std::static_pointer_cast<Entity>(boundEntity.lock());
    return entity->GetComponent(componentName);
}