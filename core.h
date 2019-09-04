#pragma once

#include <unordered_map>

namespace KibirECS {
    template<typename T, typename U>
    using Map = std::unordered_map<T, U>;

    using EntityId = uint32_t;
    using ComponentId = uint32_t;
    using SystemId = uint32_t;
}

#define GEN_COMPONENT_ID static const KibirECS::ComponentId Id() { return __COUNTER__; }
#define GEN_SYSTEM_ID static const KibirECS::SystemId Id() { return __COUNTER__; }
