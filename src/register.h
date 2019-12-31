#pragma once

#include "core.h"

namespace KibirECS {
    template<typename T>
    class Register {
    public:
        static Set<int> value;
    };

    template<typename T>
    Set<int> Register<T>::value = Set<int>{};
}
