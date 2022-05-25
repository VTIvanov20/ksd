#pragma once

#include <memory>
#include <cassert>
#include <string>

#include "../ObjectManager.hpp"

class Component : public Object
{
public:
    /**
     * @brief Define a new object of type component
     */
    DEFINE_OBJECT(Component)

    virtual ~Component() = default;
    virtual const char* ComponentName() { return nullptr; }

    bool Active = true;
    bool IsUsed = false;

    virtual void OnCreate() {}
    virtual void OnDestroy() {}
    virtual void OnUpdate() {}
    virtual void OnUI() {}

    /**
     * @brief Bind the component to an entity by pointer
     * 
     * @param object A pointer to the object to which to bind the component
     */
    void BindToEntity(std::weak_ptr<Object> object);

    /**
     * @brief Bind the component to an entity by id
     * 
     * @param id The id of the object to which to bind the component
     */
    void BindToEntity(unsigned long long _id);

    /**
     * @brief Get the component of an entity by name
     */
    std::weak_ptr<Component> GetComponent(std::string);

protected:
    std::weak_ptr<Object> boundEntity {};
};

/**
 * @brief Define a component of a type
 */
#define DEFINE_COMPONENT(TYPE) \
    TYPE(unsigned long long _id) : Component(_id) {}; \
    const char* ComponentName() override { return #TYPE; };

/**
 * @brief Define a derived component of type and the type of the base component
 */
#define DEFINE_DERIVED_COMPONENT(TYPE, BASETYPE) \
    TYPE(unsigned long long _id) : BASETYPE(_id) {}; \
    const char* ComponentName() override { return #TYPE; };

/**
 * @brief Get component by type
 */
#define MGetComponent(ComponentType) std::static_pointer_cast<ComponentType>(GetComponent(#ComponentType).lock())
#define MGetComponentFrom(From, ComponentType) std::static_pointer_cast<ComponentType>(From->GetComponent(#ComponentType).lock())