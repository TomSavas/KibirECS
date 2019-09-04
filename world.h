#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <type_traits>

#include "core.h"
#include "system.h"

namespace KibirECS {
    static EntityId entityCounter = 0;

    class World {
    private:
        Map<ComponentId, Map<EntityId, void*> > m_components;

        Map<SystemId, KibirECS::System*> m_systems;

    public:
        ~World() {
            for(auto componentMap : m_components) {
                for(auto component : componentMap.second) {
                    delete component.second;
                }
            }

            for(auto system : m_systems) {
                delete system.second;
            }
        }

        void Update(float dt) {
            for(auto system : m_systems) {
                system.second->Update(dt);
            }
        }

        template<typename T> 
        T* AddSystem() {
            T* system = new T();

            std::unordered_set<EntityId> requiredEntities;
            for(auto componentId : system->m_requirements) {

                if(requiredEntities.size() == 0) {
                    for(auto components : m_components[componentId]) {
                        requiredEntities.insert(components.first);
                    }
                } else {
                    std::unordered_set<EntityId> potentiallyRequiredEntities;

                    for(auto components : m_components[componentId]) {
                        if(requiredEntities.find(components.first) != requiredEntities.end()) {
                            potentiallyRequiredEntities.insert(components.first);
                        }
                    }
                    
                    requiredEntities = potentiallyRequiredEntities;
                }
            }

            for(auto componentId : system->m_requirements) {
                Map<EntityId, void*> components;

                for(auto entityId : requiredEntities) {
                    components[entityId] = m_components[componentId][entityId];
                }

                system->m_components.Add(componentId, components);
            }

            m_systems[T::Id()] = system;

            return system;
        }

        template<typename T>
        void RemoveSystem() {
            delete m_systems[T::Id()];

            m_systems.erase(T::Id());
        }

        template<typename T>
        T* GetSystem() {
            return m_systems[T::Id()];
        }

        EntityId CreateEntity() {
            return entityCounter++;
        }

        template<typename T>
        T* AddComponent(EntityId id) {
            auto component = new T();

            m_components[T::Id()][id] = component;

            return component;
        }

        template<typename T>
        void RemoveComponent(EntityId id) {
            delete m_components[T::Id()][id];
            m_components[T::Id()].erase(id);
        }

        template<typename T>
        T* GetComponent(EntityId id) {
            return static_cast<T*>(m_components[T::Id()][id]);
        }

        template<typename T>
        std::unordered_map<EntityId, void*> GetComponents() {
            return m_components[T::Id()];
        }
    };
}
