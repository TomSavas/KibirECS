#pragma once

#include <type_traits>

#include "core.h"
#include "system.h"
#include "entity.h"
#include "component_bundle.h"

namespace KibirECS {
    static EntityId entityCounter = 0;

    class World {
    private:
        // primary storage as components need to have layed out next to each other in memory
        Map<ComponentId, Map<EntityId, void*> > m_components;
        ComponentBundle m_componentBundle;

        // entity description objects
        Map<EntityId, Entity*> m_entityDescriptors;
        Map<SystemId, ISystem*> m_systems;

    public:
        World() : m_componentBundle(m_components) { }

        ~World() {
            for(auto componentMap : m_components) {
                for(auto component : componentMap.second) {
                    // Incorrect, deleting void*
                    delete component.second;
                }
            }

            for(auto system : m_systems) {
                delete system.second;
            }
        }

        // Eventually we'll need to separate systems into different sets to be able
        // to run them in parallel
        void Update(float dt) {
            for(auto system : m_systems) {
                system.second->Update(dt);
            }
        }

        template<typename T> 
        T* AddSystem() {
            T* system = new T();
            SystemComponents* components = new SystemComponents(m_componentBundle.GetComponents(system->GetRequirements()));

            system->BindWorld(this);
            system->BindComponents(components);

            m_systems[T::Id()] = system;
            return system;
        }

        template<typename T>
        T* GetSystem() {
            return m_systems[T::Id()];
        }

        template<typename T>
        void RemoveSystem() {
            //mem leak because of system components
            delete m_systems[T::Id()];

            m_systems.erase(T::Id());
        }

        EntityId CreateEntity() {
            m_entityDescriptors[entityCounter] = new Entity(entityCounter);

            return m_entityDescriptors[entityCounter++]->Id();
        }

        void RemoveEntity(EntityId id) {
            auto entity = m_entityDescriptors[id];
            auto components = entity->GetComponents();

            for(int i = 0; i < components.size(); i++) {
                if(components[i]) {
                    m_componentBundle.RemoveComponent(entity, i);
                    
                    // call destructor?
                    void* componentToDelete = m_components[i][id];
                    delete componentToDelete;
                    m_components[i].erase(id);
                }
            }

            delete entity;
            m_entityDescriptors.erase(id);
        }

        template<typename T>
        T* AddComponent(EntityId id) {
            T* component = new T();
            m_components[T::Id()][id] = component;
            m_entityDescriptors[id]->AddComponent<T>();
            m_componentBundle.AddComponent<T>(component, m_entityDescriptors[id]);

            return component;
        }

        template<typename T>
        void RemoveComponent(EntityId id) {
            m_componentBundle.RemoveComponent<T>(m_entityDescriptors[id]);
            m_entityDescriptors[id]->RemoveComponent<T>();

            m_components[T::Id()].remove(id);
            T* componentToDelete = static_cast<T*>(m_components[T::Id()][id]);
            delete componentToDelete;
        }

        template<typename T>
        T* GetComponent(EntityId id) {
            return static_cast<T*>(m_components[T::Id()][id]);
        }

        template<typename T>
        Map<EntityId, void*>& GetComponents() {
            return m_components[T::Id()];
        }
    };
}
