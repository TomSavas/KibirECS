#pragma once 

#include <vector>

#include "core.h"
#include "system_components.h"
#include "counter.h"

namespace KibirECS {
    class World;

    namespace Internal::RegisterIdentifiers {
        class System {};
    }

    class InternalSystem { 
    private:
        std::vector<bool> m_requirements;   

        friend World;

    protected:
        SystemComponents* m_components;
        World* m_world;

    public:
        // No template magic here, just gives a nice syntax from system's perspective
        template<typename T = void, typename... Components>
        void RequireComponents();

        virtual ~InternalSystem() {}

        virtual void Update(float dt) {}

        virtual void OnEnable() {}
        virtual void OnDisable() {}

        void BindWorld(World* world) {
            m_world = world;
        }

        void BindComponents(SystemComponents* components) {
            m_components = components;
        }

        const std::vector<bool>& GetRequirements() {
            return m_requirements;
        }
    };

    template<typename T, typename... Components>
    void KibirECS::InternalSystem::RequireComponents() {
        if(m_requirements.empty()) {
            m_requirements = std::vector<bool>(Register<Internal::RegisterIdentifiers::Component>::value.size(), false);
        }

        m_requirements[T::Id()] = true;
        RequireComponents<Components...>();
    }

    template<>
    void KibirECS::InternalSystem::RequireComponents<>() {}

    template<typename TDerived>
    class System : public Registrator<Internal::RegisterIdentifiers::System, TDerived>, public InternalSystem {
    public:
        virtual ~System() {}
        
        static int Id() {
            static int internalId = Counter<Internal::RegisterIdentifiers::System>::value++;
            return internalId;
        }
    };
}
