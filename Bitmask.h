#ifndef ECS_BITMASK_H
#define ECS_BITMASK_H

#include <bitset>
#include <typeindex>
#include <unordered_map>

/**
*  Macro that defines the number of components allowed in implementation
*  of ECS. If you wish to use a number that better fits your constraints
*  you just have to define the macro before the inclusion of this file.
*  Since this is an internal utility class you shouldn't be including it
*  manually therefore the definition of your value has to come before the
*  inclusion of any of the ECS headers.
*/
#ifndef BITMASK_MAX_COMPONENTS
    #define BITMASK_MAX_COMPONENTS 256
#endif

namespace internal
{

class Bitmask
{
public:

    Bitmask();

    void addComponent(std::type_index _t);
    void removeComponent(std::type_index _t);

    bool operator==(const Bitmask& _rhs) const;
    Bitmask operator&(const Bitmask& _rhs);

private:

    std::bitset<BITMASK_MAX_COMPONENTS> bitmask;

    static std::unordered_map<std::type_index, int> index;
    static unsigned int bitmaskSize;

    static int addNewType(std::type_index _type);
};

}

#endif // ECS_BITMASK_H
