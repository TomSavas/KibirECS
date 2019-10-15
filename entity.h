#pragma once

#include <vector>

#include "core.h"
#include "register.h"
#include "component.h"

namespace KibirECS {
    class World;

    class Entity {
    private:
        std::vector<bool> m_components;
        EntityId m_Id;

        friend World;
    public:
        Entity(EntityId id) : m_Id(id) {
            m_components = std::vector<bool>(KibirECS::Register<InternalComponent>::value.size(), false);
        }

        EntityId Id() { return m_Id; }

        const std::vector<bool>& GetComponents() { 
            return m_components;
        }

        template<typename T> 
        void AddComponent() {
            m_components[T::Id()] = true;
        }

        template<typename T> 
        void RemoveComponent() {
            m_components[T::Id()] = false;
        }

        void RemoveComponent(ComponentId componentId) {
            m_components[componentId] = false;
        }

        bool ContainsComponent(ComponentId componentId) {
            return m_components[componentId];
        }
    
        template<typename T> 
        bool ContainsComponent() {
            return ContainsComponent(T::Id());
        }
    };
}
