#pragma once

#include <memory>
#include <cassert>

#include "../ObjectManager.hpp"

class Component : public Object
{
public:
    DEFINE_OBJECT(Component)

    virtual ~Component() = default;
    virtual const char* ComponentName() { return nullptr; }

    bool Active = true;
    bool IsUsed = false;

    virtual void OnCreate() {}
    virtual void OnDestroy() {}
    virtual void OnUpdate() {}

    inline bool WantsUpdate() { return wantsUpdate; }
    void BindToEntity(std::weak_ptr<Object> object);
    void BindToEntity(uint64_t id);
    std::weak_ptr<Component> GetComponent(std::string);

protected:
    std::weak_ptr<Object> boundEntity;
    bool wantsUpdate = false;
};

#define DEFINE_COMPONENT(TYPE) \
    TYPE(uint64_t id) : Component(id) {}; \
    const char* ComponentName() override { return #TYPE; };

#define DEFINE_DERIVED_COMPONENT(TYPE, BASETYPE) \
    TYPE(uint64_t id) : BASETYPE(id) {}; \
    const char* ComponentName() override { return #TYPE; };

#define MGetComponent(ComponentType) std::static_pointer_cast<ComponentType>(GetComponent(#ComponentType).lock())
