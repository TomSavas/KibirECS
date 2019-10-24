#pragma once

#include <vector>

#include "core.h"
#include "register.h"
#include "component.h"

namespace KibirECS {
    class Entity {
    private:
        EntityId m_Id;
        std::vector<bool> m_components;

    public:
        Entity(EntityId id) : m_Id(id) {
            m_components = std::vector<bool>(
                    KibirECS::Register<Internal::RegisterIdentifiers::Component>::value.size(),
                    false);
        }

        EntityId Id() { return m_Id; }

        const std::vector<bool>& GetComponents() { 
            return m_components;
        }

        template<typename T> 
        void AddComponent() {
            m_components[T::Id()] = true;
        }

        void AddComponent(ComponentId componentId) {
            m_components[componentId] = true;
        }

        template<typename T> 
        void RemoveComponent() {
            m_components[T::Id()] = false;
        }

        void RemoveComponent(ComponentId componentId) {
            m_components[componentId] = false;
        }

        template<typename T> 
        bool ContainsComponent() {
            return m_components[T::Id()];
        }

        bool ContainsComponent(ComponentId componentId) {
            return m_components[componentId];
        }
    };
}
