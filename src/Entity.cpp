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
        auto sharedComponent = component.second.lock();
        if (sharedComponent->Active)
            sharedComponent->OnUpdate();
    }
}

void Entity::OnUpdateUIComponents()
{
    for (auto component : boundComponents)
    {
        assert(!component.second.expired() && "An expired component is kept in the boundComponents array");
        auto sharedComponent = component.second.lock();
        if (sharedComponent->Active)
            sharedComponent->OnUI();
    }
}

void Entity::OnEntityDestroy(bool checkDestroyOnReload)
{
    for (auto component : boundComponents)
    {
        assert(!component.second.expired() && "An expired component is kept in the boundComponents array");
        auto sharedComponent = component.second.lock();
        if (sharedComponent->DestroyOnReload() || !checkDestroyOnReload)
        {
            sharedComponent->OnDestroy();
            sharedComponent->IsUsed = false;
        }
    }
}

void Entity::BindComponent(std::weak_ptr<Component> component)
{
    assert(!component.expired() && "An attempt was made to bind an expired Component");
    assert(component.lock()->ComponentName() != nullptr && "An attempt was made to bind an instance of the base Component class");
    assert(!component.lock()->IsUsed && "An attempt was made to bind a used Component");

    auto sharedComponent = component.lock();

    std::string componentName = sharedComponent->ComponentName();
    assert(boundComponents.find(componentName) == boundComponents.end() && "An attempt was made to bind an already existing component type");
    
    sharedComponent->BindToEntity(this->GetID());
    boundComponents[componentName] = component;
}

std::weak_ptr<Component> Entity::GetComponent(std::string componentName)
{
    if (boundComponents.find(componentName) != boundComponents.end())
        return boundComponents[componentName];
    else return std::weak_ptr<Component>();
}