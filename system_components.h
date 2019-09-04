#pragma once

#include "core.h"

namespace KibirECS {
    class SystemComponents {
        private:
            Map<ComponentId, Map<EntityId, void*> > m_components;

        public:

        SystemComponents() {}
        
        template<typename T>
        T* Get(EntityId id) {
            return static_cast<T*>(m_components[T::Id()][id]);
        }

        template<typename T>
        Map<EntityId, void*> Get() {
            return m_components[T::Id()];
        }

        template<typename T>
        void Add(Map<EntityId, void*> entityComponentMap) {
            m_components[T::Id()] = entityComponentMap;
        }

        void Add(ComponentId componentId, Map<EntityId, void*> entityComponentMap) {
            m_components[componentId] = entityComponentMap;
        }
    };
}
