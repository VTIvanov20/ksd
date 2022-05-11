#include "ObjectManager.hpp"
#include "./util/graphics.hpp"
#include "./Entity.hpp"
#include "./components/Component.hpp"

uint64_t ObjectManager::currentId = 0;
ObjectManager* ObjectManager::instance = nullptr;

bool ObjectManager::CheckBaseName(std::weak_ptr<Object> object, const char* name)
{
    assert(!object.expired() && "Passed an expired object in CheckBaseName");
    assert(object.lock()->ObjectBaseName() && "Passed an empty object with no BaseName");
    assert(name && "Passed nullptr on second argument name");

    if (strcmp(object.lock()->ObjectBaseName(), name) == 0)
        return true;
    return false;
}

void ObjectManager::CleanUnusedComponents()
{
    std::list<uint64_t> idsForDestroying;
    for (auto object : objectTable)
    {
        if (CheckBaseName(object.second, "Component"))
        {
            auto sharedComponent = std::static_pointer_cast<Component>(object.second);
            if (!sharedComponent->IsUsed && sharedComponent->DestroyOnReload())
            {
                idsForDestroying.push_back(object.first);
                sharedComponent->OnDestroy();
            }
        }
    }

    for(auto id : idsForDestroying)
        DestroyObjectFromID(id);
}

ObjectManager* ObjectManager::GetInstance()
{
    if (instance == nullptr)
        instance = new ObjectManager();

    return instance;
}

int ObjectManager::GetObjectCount()
{
    return objectTable.size();
}

void ObjectManager::DestroyAllEntities()
{
    std::list<uint64_t> idsForDestroying;

    for (auto object : objectTable)
    {
        if (object.second->DestroyOnReload())
        {
            if (CheckBaseName(object.second, "Entity"))
                std::static_pointer_cast<Entity>(object.second)->OnEntityDestroy();
            else if (CheckBaseName(object.second, "Component"))
                printf("Destroying %s component\n", std::static_pointer_cast<Component>(object.second)->ComponentName());
            idsForDestroying.push_back(object.second->GetID());
        }
    }

    for (auto id : idsForDestroying)
        DestroyObjectFromID(id);
}

void ObjectManager::DestroyEntityFromID(uint64_t id)
{
    if (objectTable.find(id) != objectTable.end())
    {
        if (CheckBaseName(objectTable[id], "Entity"))
        {
            std::static_pointer_cast<Entity>(objectTable[id])->OnEntityDestroy();
            DestroyObjectFromID(id);
        }
    }
}

void ObjectManager::DestroyAllObjects()
{
    std::list<uint64_t> idsForDestroying;
    
    for (auto object : objectTable)
    {
        if (object.second->DestroyOnReload())
            idsForDestroying.push_back(object.second->GetID());
    }

    for (auto id : idsForDestroying)
        DestroyObjectFromID(id);
}

void ObjectManager::DestroyObjectFromID(uint64_t id)
{
    objectTable.erase(id);
}

std::weak_ptr<Object> ObjectManager::GetObjectFromID(uint64_t id)
{
    if (objectTable.find(id) != objectTable.end())
        return objectTable[id];
    else return std::weak_ptr<Object>(); /* null */
}

std::list<std::weak_ptr<Object>> ObjectManager::GetObjectsFromName(std::string name)
{
    std::list<std::weak_ptr<Object>> out {};

    for (auto [key, value] : objectTable)
    {
        if (CheckBaseName(value, name.c_str()))
            out.push_back(value);
    }

    return out;
}

std::weak_ptr<Object> ObjectManager::GetEntityFromTagName(std::string name)
{
    for (auto [key, value] : objectTable)
    {
        if (CheckBaseName(value, "Entity"))
        {
            std::shared_ptr<Entity> shEntity = std::static_pointer_cast<Entity>(value);
            if (shEntity->TagName == name)
                return value;
        }
    }

    return std::weak_ptr<Object>();
}

bool ObjectManager::TagNameAlreadyExists(std::string name)
{
    for (auto [key, value] : objectTable)
    {
        if (CheckBaseName(value, "Entity"))
        {
            std::shared_ptr<Entity> shEntity = std::static_pointer_cast<Entity>(value);
            if (shEntity->TagName == name)
                return true;
        }
    }

    return false;
}

void ObjectManager::TriggerCreateEvents()
{
    for (auto obj : objectTable)
    {
        if (CheckBaseName(obj.second, "Entity"))
        {
            std::static_pointer_cast<Entity>(obj.second)->OnEntityCreate();
        }
    }

    CleanUnusedComponents();
}

void ObjectManager::TriggerUpdateEvents()
{
    CleanUnusedComponents();

    for (auto obj : objectTable)
    {
        if (CheckBaseName(obj.second, "Entity"))
        {
            std::static_pointer_cast<Entity>(obj.second)->OnUpdateComponents();
        }
    }

    RLImGui::BeginImGuiDrawing();
    for (auto obj : objectTable)
    {
        if (CheckBaseName(obj.second, "Entity"))
        {
            std::static_pointer_cast<Entity>(obj.second)->OnUpdateUIComponents();
        }
    }
    RLImGui::EndImGuiDrawing();
}