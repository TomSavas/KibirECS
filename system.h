#pragma once 

#include <vector>

#include "core.h"
#include "registrator.h"
#include "isystem.h"
#include "system_components.h"
#include "counter.h"

namespace KibirECS {
    class World;

    namespace Internal::RegisterIdentifiers {
        class System {};
    }

    template<typename TDerived>
    class System : public Registrator<Internal::RegisterIdentifiers::System, TDerived>, 
        public ISystem {
    protected:
        SystemComponents* m_components;
        World* m_world;

    public:
        virtual ~System() {}

        void BindWorld(World* world) override {
            m_world = world;
        }

        void BindComponents(SystemComponents* components) override {
            m_components = components;
        }

        const std::vector<bool>& GetRequirements() override {
            return m_requirements;
        }

        static int Id() {
            static int internalId = Counter<Internal::RegisterIdentifiers::System>::value++;
            return internalId;
        }
    };
}
