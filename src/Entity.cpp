#include "Entity.hpp"

void Entity::OnEntityCreate()
{
    for (auto component : boundComponents)
    {
        assert(!component.second.expired() && "An expired component is kept in the boundComponents array");
        component.second.lock()->OnCreate();
    }
}

void Entity::OnUpdateComponents()
{
    for (auto component : boundComponents)
    {
        assert(!component.second.expired() && "An expired component is kept in the boundComponents array");
        if (component.second.lock()->Active || component.second.lock()->WantsUpdate())
            component.second.lock()->OnUpdate();
    }
}

void Entity::OnEntityDestroy()
{
    for (auto component : boundComponents)
    {
        assert(!component.second.expired() && "An expired component is kept in the boundComponents array");
        component.second.lock()->OnDestroy();
        component.second.lock()->IsUsed = false;
    }
}

void Entity::BindComponent(std::weak_ptr<Component> component)
{
    assert(!component.expired() && "An attempt was made to bind an expired Component");
    assert(component.lock()->ComponentName() != nullptr && "An attempt was made to bind an instance of the base Component class");
    assert(!component.lock()->IsUsed && "An attempt was made to bind a used Component");

    std::string componentName = component.lock()->ComponentName();
    assert(boundComponents.find(componentName) == boundComponents.end() && "An attempt was made to bind an already existing component type");
    
    component.lock()->BindToEntity(this->GetID());
    boundComponents[componentName] = component;
}

std::weak_ptr<Component> Entity::GetComponent(std::string componentName)
{
    if (boundComponents.find(componentName) != boundComponents.end())
        return boundComponents[componentName];
    else return std::weak_ptr<Component>();
}