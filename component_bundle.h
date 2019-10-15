#pragma once

#include "core.h"

namespace KibirECS {
    class ComponentBundle {
    private:
        Map<std::vector<bool>, Map<ComponentId, Map<EntityId, void*>>> m_componentContainers;
        Map<ComponentId, Map<EntityId, void*>>& m_availableComponents;

    public:
        ComponentBundle(Map<ComponentId, Map<EntityId, void*>>& availableComponents) : m_availableComponents(availableComponents) {}

        template<typename T>
        T* AddComponent(T* component, Entity* entity) {
            for(auto it = m_componentContainers.begin(); it != m_componentContainers.end(); it++) {
                if(!it->first[T::Id()])
                    continue;

                
                bool shouldBeInComponentMap = true;
                for(int i = 0; i < it->first.size(); i++) {
                    if(it->first[i] && !entity->ContainsComponent(i))
                    {
                        shouldBeInComponentMap = false;       
                        break;
                    }
                }
                
                if(shouldBeInComponentMap) {
                    it->second[T::Id()][entity->Id()] = component;

                    // this can later be moved to the for above
                    //
                    // check if all other components have been added
                    for(int i = 0; i < it->first.size(); i++) {
                        if(entity->ContainsComponent(i) && it->second[i].find(entity->Id()) == it->second[i].end()) {
                            it->second[i][entity->Id()] = m_availableComponents[i][entity->Id()];
                        }
                    }
                }
                printf("\n");
            }

            return component;
        }

        template<typename T>
        void RemoveComponent(Entity* entity) {
            RemoveComponent(entity, T::Id());
        }

        void RemoveComponent(Entity* entity, ComponentId componentId) {
            for(auto it = m_componentContainers.begin(); it != m_componentContainers.end(); it++) {
                if(it->first[componentId]) {
                    it->second[componentId].erase(entity->Id());
                }
            }
        }

        Map<ComponentId, Map<EntityId, void*>>* GetComponents(const std::vector<bool>& requiredComponents) {
            return &m_componentContainers[requiredComponents];
        }
    };
}
