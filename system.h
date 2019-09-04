#pragma once 

#include <vector>

#include "core.h"
#include "system_components.h"

namespace KibirECS {
    class World;

    class System {
    private:
        std::vector<ComponentId> m_requirements;   

        friend World;

    protected:
        SystemComponents m_components;

    public:
        template<typename T = void, typename... Components>
        void RequireComponents();

        virtual ~System() {}

        virtual void Update(float dt) = 0;

        virtual void OnEnable() {}
        virtual void OnDisable() {}
    };

    template<typename T, typename... Components>
    void KibirECS::System::RequireComponents() {
        m_requirements.push_back(T::Id());
        RequireComponents<Components...>();
    }

    template<>
    void KibirECS::System::RequireComponents<>() {}
}
