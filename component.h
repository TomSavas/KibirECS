#pragma once

#include "core.h"
#include "counter.h"
#include "registrator.h"

namespace KibirECS {
    namespace Internal::RegisterIdentifiers {
        class Component {};
    }

    class InternalComponent {
    public:
        virtual ~InternalComponent() {}
    };

    template<typename T>
    class Component : public Registrator<Internal::RegisterIdentifiers::Component, T>, public InternalComponent {
    public:
        virtual ~Component() {
            Registrator<Internal::RegisterIdentifiers::Component, T>::m_registered;
        }

        static int Id() {
            static int internalId = Counter<Internal::RegisterIdentifiers::Component>::value++;
            return internalId;
        }
    };
}
