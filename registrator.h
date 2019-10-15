#pragma once

#include "register.h"

namespace KibirECS {
    template<typename TGroup, typename TObject>
    class Registrator {
    protected:
        static bool m_registered;
    };

    template<typename TGroup, typename TObject>
    bool Registrator<TGroup, TObject>::m_registered = KibirECS::Register<TGroup>::value.insert(TObject::Id()).second;
}

