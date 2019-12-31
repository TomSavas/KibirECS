#pragma once

#include <type_traits>

namespace KibirECS {
    template<typename T>
    struct Counter {
        static int value;
    };

    template<typename T>
    int Counter<T>::value = 0;
}
