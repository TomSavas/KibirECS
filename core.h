#pragma once

#include <map>
#include <unordered_set>

#ifndef KB_ENTITY_ID
#define KB_ENTITY_ID uint32_t
#endif

#ifndef KB_COMPONENT_ID
#define KB_COMPONENT_ID uint32_t
#endif

#ifndef KB_SYSTEM_ID
#define KB_SYSTEM_ID uint32_t
#endif

// Eventually this will be replaced with a cache-friendly map
#ifndef KB_MAP
#define KB_MAP std::map
#endif

#ifndef KB_SET
#define KB_SET std::unordered_set
#endif

#ifndef KB_MAX_COMPONENTS_PER_SYSTEM
#define KB_MAX_COMPONENTS_PER_SYSTEM 256
#endif

namespace KibirECS {
    template<typename T, typename U>
    using Map = KB_MAP<T, U>;

    template<typename T>
    using Set = KB_SET<T>;

    using EntityId = KB_ENTITY_ID;
    using ComponentId = KB_COMPONENT_ID;
    using SystemId = KB_SYSTEM_ID;
    using Hash = size_t;

    static int MAX_COMPONENTS_PER_SYSTEM = KB_MAX_COMPONENTS_PER_SYSTEM;
}
