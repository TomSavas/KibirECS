#pragma once

#include <type_traits>

namespace KibirECS {
    template<typename T>
    struct Counter {
        //static_assert(std::is_integral<T>::value, "Counter can only be used with integral types.");
        static int value;
    };

    template<typename T>
    int Counter<T>::value = 0;
}
