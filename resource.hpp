#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <string>
#include "resource_type.hpp"

namespace ariel {
    
    class Resource 
    {
        private:
            ResourceType type;
            int quantity;

        protected:
            void setQuantity(int value) 
            {
                quantity = value;
            }
            
            int getQuantityInternal() const 
            {
                return quantity;
            }

        public:
            Resource(ResourceType type, int quantity = 0) : type(type), quantity(quantity) {}
            virtual void addQuantity(int value) = 0;
            virtual void removeQuantity(int value) = 0;

            ResourceType getType() const;
            int getQuantity() const;
    };


    class Wood : public Resource 
    {
        public:
            Wood(int quantity = 0) : Resource(ResourceType::WOOD, quantity) {}
            void addQuantity(int value) override;
            void removeQuantity(int value) override;
    };

    class Brick : public Resource 
    {
        public:
            Brick(int quantity = 0) : Resource(ResourceType::BRICK, quantity) {}
            void addQuantity(int value) override;
            void removeQuantity(int value) override;
    };

    class Wool : public Resource 
    {
        public:
            Wool(int quantity = 0) : Resource(ResourceType::WOOL, quantity) {}
            void addQuantity(int value) override;
            void removeQuantity(int value) override;
    };

    class Grain : public Resource 
    {
        public:
            Grain(int quantity = 0) : Resource(ResourceType::GRAIN, quantity) {}
            void addQuantity(int value) override;
            void removeQuantity(int value) override;
    };

    class Ore : public Resource 
    {
        public:
            Ore(int quantity = 0) : Resource(ResourceType::ORE, quantity) {}
            void addQuantity(int value) override;
            void removeQuantity(int value) override;
    };
}

#endif