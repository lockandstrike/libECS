#ifndef ECS_ENTITY_H
#define ECS_ENTITY_H

#include <memory>
#include <stdexcept>

#include <ECS/Bitmask.h>
#include <ECS/Component.h>
#include <ECS/EntityManager.h>

/*
*   Entity is a wrapper for the id which will provide some aditional functionality
*   to check the validity and state of the components associated with it and help
*   with comunication between components/entities.
*
*   TODO: Provide extra functionality
*/

class Entity
{
public:

    const unsigned int id;

    template <typename T, typename... Args>
    Handle<T> add(Args&&... _args)
    {
        if(this->alive)
        {
            this->bitmask.addComponent(std::type_index(typeid(T)));
            return manager->newComponent<T>(this, _args...);
        }
        throw std::runtime_error("Attempt to add a component to an entity that has already been destroyed");
    }

    template <typename T>
    void remove()
    {
        if(this->alive)
        {
            this->bitmask.removeComponent(std::type_index(typeid(T)));
            manager->deleteComponent<T>(this);
        }
        throw std::runtime_error("Attemp to remove a component to an entity that has already been destroyed");
    }

private:

    friend EntityManager;
    Entity(unsigned int _id, EntityManager* _manager)
    :
    alive(true),
    id(_id),
    manager(_manager)
    {
    }

    bool alive;
    internal::Bitmask bitmask;
    EntityManager* manager;

};

#endif // ECS_ENTITY_H
