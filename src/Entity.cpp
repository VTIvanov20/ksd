#include "Entity.hpp"

void Entity::OnEntityCreate()
{
    /**
     * @brief Gets every component in the component bool and calls the OnCreate() function
     */
    for (auto component : boundComponents)
    {
        assert(!component.second.expired() && "An expired component is kept in the boundComponents array");
        component.second.lock()->OnCreate();
    }
}

void Entity::OnUpdateComponents()
{
    /**
     * @brief Gets every component in the component bool and calls the OnUpdate() function if they're active
     */
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
    /**
     * @brief Gets every component in the component bool and calls the OnUI() function if they're active
     */
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
    /**
     * @brief Gets every component in the component bool and calls the OnDestroy() function if it's destroyed
     */
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

    /**
     * @brief Creates a shared_ptr of the component parameter
     */
    auto sharedComponent = component.lock();

    /**
     * @brief Sets the component name and binds it to the entity in which it's used, then adds it to the component pool
     */
    std::string componentName = sharedComponent->ComponentName();
    assert(boundComponents.find(componentName) == boundComponents.end() && "An attempt was made to bind an already existing component type");
    
    sharedComponent->BindToEntity(this->GetID());
    boundComponents[componentName] = component;
}

std::weak_ptr<Component> Entity::GetComponent(std::string componentName)
{
    /**
     * @brief If the component is found in the pool, it returns it, else it returns empty
     */
    if (boundComponents.find(componentName) != boundComponents.end())
        return boundComponents[componentName];
    else return std::weak_ptr<Component>();
}