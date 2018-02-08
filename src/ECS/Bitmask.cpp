#include <ECS/Bitmask.h>

#include <stdexcept>

namespace internal
{
    Bitmask::Bitmask()
    {
    }

    void Bitmask::addComponent(std::type_index _t)
    {
        int bit;
        auto it = index.find(_t);
        if(it == index.end())
        {
            bit = this->addNewType(_t);
        }
        else
        {
            bit = it->second;
        }

        this->bitmask[bit] = 1;
    }

    void Bitmask::removeComponent(std::type_index _t)
    {
        int bit;
        auto it = index.find(_t);
        if(it == index.end())
        {
            throw std::runtime_error("Attempt to delete a component that hasn't been registered in bitmask table");
        }
        else
        {
            bit = it->second;
        }

        this->bitmask[bit] = 0;
    }

    bool Bitmask::operator==(const Bitmask& _rhs) const
    {
        return this->bitmask == _rhs.bitmask;
    }

    Bitmask Bitmask::operator&(const Bitmask& _rhs)
    {
        Bitmask result;

        result.bitmask = this->bitmask & _rhs.bitmask;

        return result;
    }

    std::unordered_map<std::type_index, int> Bitmask::index = {};
    unsigned int Bitmask::bitmaskSize = 0;

    int Bitmask::addNewType(std::type_index _type)
    {
        if(bitmaskSize < BITMASK_MAX_COMPONENTS)
        {
            auto it = index.insert({_type, bitmaskSize++});
            return it.first->second;
        }
        else
        {
            throw std::runtime_error("Max components exceded");
        }
    }

}
