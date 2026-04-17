/**
 * @file PlatformHandle.ixx
 * @brief Handle alias for runtime platform entities.
 */
module;

export module helios.platform.runtime.types.PlatformHandle;

import helios.ecs.types.EntityHandle;
import helios.platform.runtime.types.PlatformId;

using namespace helios::ecs::types;
export namespace helios::platform::runtime::types {

    /**
     * @brief ECS handle type referencing runtime platform entities.
     */
    using PlatformHandle = helios::ecs::types::EntityHandle<PlatformId>;

}