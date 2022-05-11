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
    std::unordered_map<std::string, std::weak_ptr<Component>> boundComponents{};
    
public:
    DEFINE_OBJECT(Entity);

    virtual ~Entity() = default;

    void OnEntityCreate(); // Gets called once whenever the program is initializing itself
    void OnUpdateComponents(); // Gets called once per frame
    void OnUpdateUIComponents(); // Gets called once per frame
    void OnEntityDestroy(); // Gets called when destroyed

    void BindComponent(std::weak_ptr<Component> component);
    std::weak_ptr<Component> GetComponent(std::string componentName);

    std::string TagName {};

    bool DestroyOnReload() override { return _destroyOnReload; };

    bool _destroyOnReload = true;
};