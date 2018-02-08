libECS is a simple and memory efficient C++11 ECS implementation.

It handles ECS in it's purest form and as such does not provide a solution for communication
between systems or between the systems and the entity manager.

It can be compiled into a linkable library or it you can just add the code into your project
directly since it only comprises of six files.

It is fairly simple to use:

```cpp
#include <ECS/Entity.h>
#include <ECS/EntityManager.h>

struct Position
{
  Position(float _x, float _y)
  :
  x(_x),
  y(_y)
  {}

  float x;
  float y;
};

void doStuff(Handle<Position> _position)
{
  _position->x += 1.0f;
}

int main()
{
  //Instanciate an EntityManager object
  EntityManager manager;
  
  //Create an entity and retrieve a pointer to it
  Entity* entity1 = manager.create();
  //Alternative method that is less verbose
  auto entity2 = manager.create();
  
  //Add a component to the entity you've just created and retrieve it's Handle (fancy pointer wrapper)
  //The *add* function takes as arguments the arguments of whichever constructor the type provides.
  Handle<Position> position1 = entity1.add<Position>(1.0f, 1.0f);
  //Less verbose alternative
  auto position2 = entity2.add<Position>(2.0f, 3.0f);
  
  //Call a function on every entity that has a Component of type Position that the EntityManager owns.
  //The *each* function takes as argument a function pointer to run on every entity that has the required type
  manager.each<Position>(doStuff);
  //Alternatively, it also supports lambdas
  manager.each<Position>([](Handle<Position> _position)
                         {
                            _position->y += 1.0f;
                         });
  /*
  The use of the *each* function requires a specific function prototype:
    void function(Handle<T> _t, Handle<Args> _args...);
      |                ||
     The function must have void return type
                       ||
                     It must take as arguments the Handle's to the types of Component's you're running the function for(it can
                     take more than one component type)
  */
}
```
