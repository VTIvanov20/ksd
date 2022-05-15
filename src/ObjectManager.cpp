#include "ObjectManager.hpp"
#include "./util/graphics.hpp"
#include "./Entity.hpp"
#include "./components/Component.hpp"

/**
 * @brief The current id for the latest object in the object pool
 */
uint64_t ObjectManager::currentId = 0;

/**
 * @brief Creates a null pointer instance of the singleton class ObjectManager
 */
ObjectManager* ObjectManager::instance = nullptr;

bool ObjectManager::CheckBaseName(std::weak_ptr<Object> object, const char* name)
{
    assert(!object.expired() && "Passed an expired object in CheckBaseName");
    assert(object.lock()->ObjectBaseName() && "Passed an empty object with no BaseName");
    assert(name && "Passed nullptr on second argument name");

    /**
     * @brief Checks if the name of the object is the same as the name param
     */
    if (strcmp(object.lock()->ObjectBaseName(), name) == 0)
        return true;
    return false;
}

void ObjectManager::CleanUnusedComponents()
{
    /**
     * @brief Creates a list with ids to destroy, checks if they are components and if they are unused adds them to it and then calls the OnDestroy() function
     */
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

    /**
     * @brief Destroys every object in the idsForDestroying list
     */
    for(auto id : idsForDestroying)
        DestroyObjectFromID(id);
}

ObjectManager* ObjectManager::GetInstance()
{
    /**
     * @brief If an instance of the singleton doesn't exist, creates one
     */
    if (instance == nullptr)
        instance = new ObjectManager();

    return instance;
}

int ObjectManager::GetObjectCount()
{
    /**
     * @brief Gets the size of the object pool map and returns it
     */
    return objectTable.size();
}

void ObjectManager::DestroyAllEntities(bool checkDestroyOnReload)
{
    /**
     * @brief Creates a list with ids to destroy, checks if they are entities and if they are adds them to it and then calls the OnEntityDestroy() function
     */
    std::list<uint64_t> idsForDestroying;

    for (auto object : objectTable)
    {
        if (object.second->DestroyOnReload() || !checkDestroyOnReload)
        {
            if (CheckBaseName(object.second, "Entity"))
                std::static_pointer_cast<Entity>(object.second)->OnEntityDestroy(checkDestroyOnReload);
            else if (CheckBaseName(object.second, "Component"))
                printf("Destroying %s component\n", std::static_pointer_cast<Component>(object.second)->ComponentName());
            idsForDestroying.push_back(object.second->GetID());
        }
    }

    /**
     * @brief Destroys every entity in the idsForDestroying list
     */
    for (auto id : idsForDestroying)
        DestroyObjectFromID(id);
}

void ObjectManager::DestroyEntityFromID(uint64_t id)
{
    /**
     * @brief Goes through the object pool and if it finds the id we are looking for, destroys it
     */
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
    /**
     * @brief Creates a list with ids to destroy and then adds them to it
     */
    std::list<uint64_t> idsForDestroying;
    
    for (auto object : objectTable)
    {
        if (object.second->DestroyOnReload())
            idsForDestroying.push_back(object.second->GetID());
    }

    /**
     * @brief Destroys every entity in the idsForDestroying list
     */
    for (auto id : idsForDestroying)
        DestroyObjectFromID(id);
}

void ObjectManager::DestroyObjectFromID(uint64_t id)
{
    /**
     * @brief Destroys the object in the object pool by id
     */
    objectTable.erase(id);
}

std::weak_ptr<Object> ObjectManager::GetObjectFromID(uint64_t id)
{
    /**
     * @brief Goes through the object pool and if it finds the id we are looking for, returns it. If not, returns empty
     */
    if (objectTable.find(id) != objectTable.end())
        return objectTable[id];
    else return std::weak_ptr<Object>(); /* null */
}

std::list<std::weak_ptr<Object>> ObjectManager::GetObjectsFromName(std::string name)
{
    /**
     * @brief Creates a list for the wanted objects. Goes through the object pool and if it finds the objects with the names we are looking for, returns them. If not, returns empty
     */
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
    /**
     * @brief Goes through the object pool and if it finds the entity with the tag name we are looking for, returns it. If not, returns empty
     */
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
    /**
     * @brief Goes through the object pool and if it finds an entity with the tag name param, returns true. If not, returns false
     */
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
    /**
     * @brief Goes through the object pool and if it finds an entity, calls the OnEntityCreate() function for it
     */
    for (auto obj : objectTable)
    {
        if (CheckBaseName(obj.second, "Entity"))
        {
            std::static_pointer_cast<Entity>(obj.second)->OnEntityCreate();
        }
    }

    /**
     * @brief Destroys all unused components
     */
    CleanUnusedComponents();
}

void ObjectManager::TriggerUpdateEvents()
{
    /**
     * @brief Destroys all unused components
     */
    CleanUnusedComponents();

    /**
     * @brief Goes through the object pool and if it finds an entity, calls the OnUpdateComponents() function for it
     */
    for (auto obj : objectTable)
    {
        if (CheckBaseName(obj.second, "Entity"))
        {
            std::static_pointer_cast<Entity>(obj.second)->OnUpdateComponents();
        }
    }

    /**
     * @brief Goes through the object pool and if it finds an entity, calls the OnUpdateUIComponents() function for it
     */
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