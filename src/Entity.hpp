#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <list>
#include <cassert>
#include <map>

#include "./components/Component.hpp"
#include "./ObjectManager.hpp"

class Entity : public Object
{
private:
    /**
     * @brief An unordered map as the component pool
     */
    std::unordered_map<std::string, std::weak_ptr<Component>> boundComponents{};
    
public:
    /**
     * @brief Create a new object of type entity
     */
    DEFINE_OBJECT(Entity)

    virtual ~Entity() = default;

    /**
     * @brief Gets called once whenever the program is initializing itself
     */
    void OnEntityCreate();

    /**
     * @brief Updates the components once per frame
     */
    void OnUpdateComponents();

    /**
     * @brief Updates the UI components once per frame
     */
    void OnUpdateUIComponents();

    /**
     * @brief Gets called when an entity is destroyed
     * 
     * @param checkDestroyOnReload Whether to check the DestroyOnReload() function
     */
    void OnEntityDestroy(bool checkDestroyOnReload = true);

    /**
     * @brief Bind the component parameter as a component of an entity
     * 
     * @param component Gets the component we want
     */
    void BindComponent(std::weak_ptr<Component> component);

    /**
     * @brief Gets the wanted component
     * 
     * @param componentName Finds the wanted component by its name
     * 
     * @return Returns a weak_ptr to the wanted component
     */
    std::weak_ptr<Component> GetComponent(std::string componentName);

    std::string TagName {};

    bool DestroyOnReload() override { return _destroyOnReload; };

    bool _destroyOnReload = true;
};