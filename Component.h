#ifndef ECS_COMPONENT_H
#define ECS_COMPONENT_H

class Entity;

template <typename T>
class Handle
{
public:

    Handle(T* _component)
    :
    component(_component)
    {
    }

    ~Handle()
    {

    }

    T* operator->() const
    {
        return component;
    }

private:

    T* component;

};

namespace internal
{
    struct BaseComponent
    {
        virtual ~BaseComponent() {}
    };
}

template <typename T>
class Component : public internal::BaseComponent
{
public:

    template <typename... Args>
    Component(Entity* _entity, Args&... _args)
    :
    entity(_entity),
    component(_args...)
    {
    }

    ~Component()
    {
        entity = nullptr;
    }

    Handle<T> getHandle()
    {
        return Handle<T>(&component);
    }

    Entity* getEntity()
    {
        return entity;
    }

private:

    Entity* entity;
    T component;

};

#endif // ECS_COMPONENT_H
