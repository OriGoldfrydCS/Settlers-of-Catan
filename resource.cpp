#include "resource.hpp"

namespace ariel {

    ResourceType Resource::getType() const 
    {
        return this->type;
    }

    int Resource::getQuantity() const 
    {
        return this->quantity;
    }

    void Wood::addQuantity(int value) 
    {
        setQuantity(getQuantityInternal() + value);
    }

    void Wood::removeQuantity(int value) 
    {
        setQuantity(getQuantityInternal() - value);
    }

    void Brick::addQuantity(int value) 
    {
        setQuantity(getQuantityInternal() + value);
    }

    void Brick::removeQuantity(int value) 
    {
        setQuantity(getQuantityInternal() - value);
    }

    void Wool::addQuantity(int value) 
    {
        setQuantity(getQuantityInternal() + value);
    }

    void Wool::removeQuantity(int value) 
    {
        setQuantity(getQuantityInternal() - value);
    }

    void Grain::addQuantity(int value) 
    {
        setQuantity(getQuantityInternal() + value);
    }

    void Grain::removeQuantity(int value) 
    {
        setQuantity(getQuantityInternal() - value);
    }

    void Ore::addQuantity(int value) 
    {
        setQuantity(getQuantityInternal() + value);
    }

    void Ore::removeQuantity(int value) 
    {
        setQuantity(getQuantityInternal() - value);
    }

}
