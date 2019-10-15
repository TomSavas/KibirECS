#pragma once

#include "core.h"
#include "counter.h"
#include "registrator.h"

namespace KibirECS {
    class InternalComponent {
    public:
        virtual ~InternalComponent() {}
    };

    template<typename T>
    class Component : public Registrator<InternalComponent, T>, public InternalComponent {
    public:
        virtual ~Component() {
            Registrator<InternalComponent, T>::m_registered;
        }

        static int Id() {
            static int internalId = Counter<InternalComponent>::value++;
            return internalId;
        }
    };
}
