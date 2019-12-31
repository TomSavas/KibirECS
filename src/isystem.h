#pragma once 

#include <vector>

#include "core.h"
#include "system_components.h"
#include "register.h"

namespace KibirECS {
    class World;

    class ISystem { 
    protected:
        std::vector<bool> m_requirements;   

    public:
        // No template magic here, just gives a nice syntax from system's perspective
        //
        // Although the issue here is that I'm not able to move this method to System<T>
        // because I fail defining 
        // template<TDerived> template<> void System<TDerived>::RequireComponents<>() {}
        // for whatever magical template-voodoo magic reasons.
        // This also forces m_requirements to be on the interface which is just wrong...
        template<typename T = void, typename... Components>
        void RequireComponents();

        virtual ~ISystem() {}

        virtual void Update(float dt) {}

        virtual void OnEnable() {}
        virtual void OnDisable() {}

        virtual void BindWorld(World* world) = 0;
        virtual void BindComponents(SystemComponents* components) = 0;
        virtual const std::vector<bool>& GetRequirements() = 0;
    };

    template<typename T, typename... Components>
    void KibirECS::ISystem::RequireComponents() {
        if(m_requirements.empty()) {
            m_requirements = std::vector<bool>(Register<Internal::RegisterIdentifiers::Component>
                    ::value.size(), false);
        }

        m_requirements[T::Id()] = true;
        RequireComponents<Components...>();
    }

    template<>
    void KibirECS::ISystem::RequireComponents<>() {}
}
