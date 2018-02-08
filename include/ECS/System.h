#ifndef ECS_SYSTEM_H
#define ECS_SYSTEM_H

#include <ECS/EntityManager.h>
/*
*   Abstract base class from which all user-defined systems inherit from. Is the
*   interface for component updates and interactions.
*/

class System
{
public:

    System(EntityManager* _manager)
    :
    manager(_manager)
    {}

    virtual ~System()
    {
        manager = nullptr;
    }

    //Required method that will update the components. Should be called every frame.
    virtual void update(float _dt) = 0;

protected:

    EntityManager* manager;

};

#endif // ECS_SYSTEM_H
