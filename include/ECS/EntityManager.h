#ifndef ECS_ENTITYMANAGER_H
#define ECS_ENTITYMANAGER_H

#include <functional>
#include <memory>
#include <stdexcept>
#include <typeindex>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include <ECS/Bitmask.h>
#include <ECS/Component.h>

class Entity;

/*
*   EntityManager shall own all entities and also any components associated with
*   these entities. Provides an interface for retrieving all the entities which
*   have a certain component or combination of components. Responsible for creating
*   and destroying entities.
*/

class EntityManager
{
public:

    EntityManager();

    ~EntityManager();

    /*
    *  Creates an entity and returns a pointer to it.
    */
    Entity* create();

    /*
    *  Destroys provided entity and returns the status of the destruction.
    */
    bool destroy(Entity* _entity);

    /*
    *  Runs provided function on all entities with components Args.
    */
    template <typename T, typename... Args>
    bool each(typename std::common_type<std::function<void(Handle<T>, Handle<Args>...)>>::type _function)
    {
        internal::Bitmask mask;

        mask.addComponent(std::type_index(typeid(T)));
        {
            using expander = int[];
            expander{0, ( (void) std::forward<Args>(mask.addComponent(std::type_index(typeid(Args)))), 0)...};
        }


        std::vector<Entity*> validEntities = validateMask(mask);

        if(!validEntities.empty())
        {
            for(auto i : validEntities)
            {
                _function(this->getHandle<T>(i), this->getHandle<Args>(i)...);
            }
        }
    }

    /*
    *  Adds component to the correct group and returns a handle to it
    */
    template <typename T, typename... Args>
    Handle<T> newComponent(Args&&... _args)
    {
        auto it = components.find(std::type_index(typeid(T)));
        if(it == components.end())
        {
            components[std::type_index(typeid(T))].push_back(new Component<T>(_args...));
            return static_cast<Component<T>*>(components[std::type_index(typeid(T))].back())->getHandle();
        }
        else
        {
            if(available[std::type_index(typeid(T))].empty())
            {
                it->second.push_back(new Component<T>(_args...));
            }
            else
            {
                it->second.insert(available[std::type_index(typeid(T))][0], new Component<T>(_args...));
            }
            return static_cast<Component<T>*>(it->second.back())->getHandle();
        }
    }

    /*
    *  Removes the component of type T from _entity. Throws an exception if the component type has not been
    *  registered with the EntityManager or if the Entity selected has no component of that type.
    */
    template <typename T>
    void deleteComponent(Entity* _entity)
    {
        auto it = components.find(std::type_index(typeid(T)));
        if(it == components.end())
        {
            throw std::runtime_error("Attempt to delete a component of a type that doesn't exist");
        }
        else
        {
            bool exists = false;
            for(auto i = it->second.begin(); i != it->second.end(); i++)
            {
                auto component = static_cast<Component<T>*>(*i);
                if(component->getEntity() == _entity)
                {
                    component->~Component();
                    delete *i;
                    available[std::type_index(typeid(T))].push_back(i);
                    exists = true;
                }
            }
            if(!exists)
            {
                throw std::runtime_error("Attempt to delete a component that doesn't exist");
            }
        }
    }

private:

    std::unordered_map<std::type_index, std::vector<internal::BaseComponent*>> components;
    std::unordered_map<std::type_index, std::vector<std::vector<internal::BaseComponent*>::iterator>> available;
    std::vector<Entity*> entities;
    unsigned int entityCounter;

    /*
    *  Internal function that returns Handle of component type T of _entity
    */
    template<typename T>
    Handle<T> getHandle(Entity* _entity)
    {
        auto it = components.find(std::type_index(typeid(T)));
        for(auto i: it->second)
        {
            auto component = static_cast<Component<T>*>(i);
            if(component->getEntity() == _entity)
            {
                return component->getHandle();
            }
        }
    }

    std::vector<Entity*> validateMask(internal::Bitmask bitmask);

};

#endif // ECS_ENTITYMANAGER_H
