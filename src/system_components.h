#pragma once

#include "core.h"

namespace KibirECS {
    class SystemComponents {
        private:
            Map<ComponentId, Map<EntityId, void*>>* m_components;

        public:

        SystemComponents(Map<ComponentId, Map<EntityId, void*>>* components) {
            m_components = components;
        }
        
        template<typename TComponent>
        Map<EntityId, void*>* Get() {
            return &(m_components->operator[](TComponent::Id()));
        }

        template<typename TComponent>
        TComponent* Get(EntityId id) {
            return static_cast<TComponent*>(Get<TComponent>()->operator[](id));
        }
    };
}
