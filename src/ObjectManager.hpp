#pragma once

#include <list>
#include <unordered_map>
#include <memory>
#include <cstring>
#include <cassert>
#include <cstdio>
#include <type_traits>
#include <vector>

class Object
{
private:
    uint64_t id;
    
public:
    Object(uint64_t id) : id(id) {};
    virtual ~Object()
    {
        printf("Destroyed Object with id %lld\n", id);
    }

    virtual const char* ObjectBaseName() { return nullptr; }
    inline uint64_t GetID() { return id; }

    bool DestroyOnReload = true;
};

#define DEFINE_OBJECT(TYPE) \
    TYPE(uint64_t id) : Object(id) {}; \
    const char* ObjectBaseName() override { return #TYPE; };

class ObjectManager
{
private:
    std::unordered_map<uint64_t, std::shared_ptr<Object>> objectTable {};

    static uint64_t currentId;
    static ObjectManager* instance;

    void CleanUnusedComponents();

    ObjectManager() : objectTable() {};
    ~ObjectManager() = default;

public:
    // disallow cloning singleton
    ObjectManager(ObjectManager const&) = delete;

    // disallow assignments to singleton
    void operator=(ObjectManager const&) = delete;

    static ObjectManager* GetInstance();
    static bool CheckBaseName(std::weak_ptr<Object> object, const char* name);

    template <typename ObjectType>
    std::shared_ptr<ObjectType> CreateObject()
    {
        static_assert(std::is_base_of<Object, ObjectType>::value, "ObjectType must inherit from Object");

        std::shared_ptr<ObjectType> current = std::make_shared<ObjectType>(currentId);
        objectTable[currentId] = std::static_pointer_cast<Object>(current);
        currentId++;
        return current;
    }

    int GetObjectCount();

    void DestroyAllEntities();
    void DestroyEntityFromID(uint64_t id);

    void DestroyAllObjects();
    void DestroyObjectFromID(uint64_t id);
    
    std::weak_ptr<Object> GetObjectFromID(uint64_t id);
    std::list<std::weak_ptr<Object>> GetObjectsFromName(std::string name);

    std::weak_ptr<Object> GetEntityFromTagName(std::string name);

    void TriggerCreateEvents();
    void TriggerUpdateEvents();
};
