/**
 * @file PlatformEntityManager.ixx
 * @brief ECS registry and manager aliases for runtime platform entities.
 */
module;


#include <cstddef>
#include "helios/helios_config.h"

export module helios.platform.environment.PlatformEntityManager;

import helios.ecs;

import helios.platform.environment.types;

constexpr size_t PLATFORM_SIZE = 1;

using namespace helios::ecs;
using namespace helios::ecs::strategies;
using namespace helios::platform::environment::types;
export namespace helios::platform::environment {

    /**
     * @brief Registry type for runtime platform entity lifecycle management.
     */
    using PlatformEntityRegistry = EntityRegistry<PlatformId, LinearLookupStrategy<PLATFORM_SIZE>, false, PLATFORM_SIZE>;

    /**
     * @brief Entity manager type for runtime platform components.
     */
    using PlatformEntityManager = EntityManager<PlatformHandle, PlatformEntityRegistry, PLATFORM_SIZE>;

}