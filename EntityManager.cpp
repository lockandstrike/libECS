#include <ECS/EntityManager.h>

#include <ECS/Entity.h>

EntityManager::EntityManager()
:
entityCounter(0)
{
}

EntityManager::~EntityManager()
{
    for(auto i : components)
    {
        for(auto j : i.second)
        {
            delete j;
        }
    }

    for(auto i : entities)
    {
        delete i;
    }
}

Entity* EntityManager::create()
{
    entities.push_back(new Entity(++entityCounter, this));
    return entities.back();
}

bool EntityManager::destroy(Entity* _entity)
{
    _entity->alive = false;
    _entity->manager = nullptr;
}

std::vector<Entity*> EntityManager::validateMask(internal::Bitmask bitmask)
{
    std::vector<Entity*> validEntities;
    for(auto i : entities)
    {
        if((i->bitmask & bitmask) == bitmask && i->alive)
        {
            validEntities.push_back(i);
        }
    }

    return validEntities;
}
