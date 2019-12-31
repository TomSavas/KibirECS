#pragma once

#include "core.h"
#include "counter.h"
#include "registrator.h"

namespace KibirECS {
    namespace Internal::RegisterIdentifiers {
        class Component {};
    }

    template<typename TDerived>
    class Component : public Registrator<Internal::RegisterIdentifiers::Component, TDerived> {
    public:
        virtual ~Component() {
            // If this is not called, the registrator doesn't actually register the component.
            Registrator<Internal::RegisterIdentifiers::Component, TDerived>::m_registered;
        }

        static int Id() {
            static int internalId = Counter<Internal::RegisterIdentifiers::Component>::value++;
            return internalId;
        }
    };
}
