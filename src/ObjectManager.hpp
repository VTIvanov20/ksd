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
    /**
     * @brief The id of the object
     */
    uint64_t id;
    
public:
    /**
     * @brief Sets the id of the object when it's created
     */
    Object(uint64_t id) : id(id) {};
    virtual ~Object()
    {
        printf("Destroyed Object with id %lld\n", id);
    }

    virtual const char* ObjectBaseName() { return nullptr; }
    virtual bool DestroyOnReload() { return true; }
    inline uint64_t GetID() { return id; }
};

/**
 * @brief Creates a new object and sets its id
 */
#define DEFINE_OBJECT(TYPE) \
    TYPE(uint64_t id) : Object(id) {}; \
    const char* ObjectBaseName() override { return #TYPE; };

class ObjectManager
{
private:
    /**
     * @brief An unordered map for the object pool
     */
    std::unordered_map<uint64_t, std::shared_ptr<Object>> objectTable {};

    static uint64_t currentId;
    static ObjectManager* instance;

    void CleanUnusedComponents();

    ObjectManager() : objectTable() {};
    ~ObjectManager() = default;

public:
    /**
     * @brief Diallows cloning of the singleton
     */
    ObjectManager(ObjectManager const&) = delete;

    /**
     * @brief Disallows assigning of the singleton
     */
    void operator=(ObjectManager const&) = delete;

    /**
     * @brief Gets the instance of the singleton class ObjectManager
     */
    static ObjectManager* GetInstance();

    /**
     * @brief Checks if the object parameters name is the same as the name parameter
     * 
     * @param object The object whose name to check
     * 
     * @param name The name to check
     * 
     * @return Returns whether the object parameter has the name parameter as name
     */
    static bool CheckBaseName(std::weak_ptr<Object> object, const char* name);

    /**
     * @brief Creates an object with an id, adds it to the object pool, moves on to the next id and returns the object
     * 
     * @return Returns a shared_ptr of the newly-created object
     */
    template <typename ObjectType>
    std::shared_ptr<ObjectType> CreateObject()
    {
        static_assert(std::is_base_of<Object, ObjectType>::value, "ObjectType must inherit from Object");

        std::shared_ptr<ObjectType> current = std::make_shared<ObjectType>(currentId);
        objectTable[currentId] = std::static_pointer_cast<Object>(current);
        currentId++;
        return current;
    }

    /**
     * @brief Gets the amount of objects in the object pool
     */
    int GetObjectCount();

    /**
     * @brief Destroys all of the objects in the object pool
     * 
     * @param checkDestroyOnReload Whether to check the DestroyOnReload() function
     */
    void DestroyAllEntities(bool checkDestroyOnReload = true);

    /**
     * @brief Destroys an entity by id
     * 
     * @param id The id of the entity we want to destroy
     */
    void DestroyEntityFromID(uint64_t id);

    /**
     * @brief Destroys all objects in the object pool
     */
    void DestroyAllObjects();

    /**
     * @brief Destroys an object of the object pool by id
     * 
     * @param id The id of the object we want to destroy
     */
    void DestroyObjectFromID(uint64_t id);
    
    /**
     * @brief Gets an object by id in the object pool and returns it
     * 
     * @param id The id of the object we need
     * 
     * @return Returns the object it gets from the id param
     */
    std::weak_ptr<Object> GetObjectFromID(uint64_t id);

    /**
     * @brief Gets an object by name in the object pool and returns it
     * 
     * @param name The name of the object we need
     * 
     * @return Returns the object it gets from the name param
     */
    std::list<std::weak_ptr<Object>> GetObjectsFromName(std::string name);

    /**
     * @brief Gets an object by tag name in the object pool and returns it
     * 
     * @param name The tag name of the object we need
     * 
     * @return Returns the object it gets from the tag name
     */
    std::weak_ptr<Object> GetEntityFromTagName(std::string name);

    /**
     * @brief Returns if an object with a tag name already exists in the object pool
     * 
     * @param name The tag name which we need to find out if exists
     * 
     * @return Returns whether the tag name already exists
     */
    bool TagNameAlreadyExists(std::string name);

    /**
     * @brief Creates all drawables
     */
    void TriggerCreateEvents();

    /**
     * @brief Draws and updates all drawables
     */
    void TriggerUpdateEvents();
};
