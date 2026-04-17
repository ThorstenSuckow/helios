/**
 * @file PlatformId.ixx
 * @brief Strong-id type definition for runtime platform entities.
 */
module;

export module helios.platform.runtime.types.PlatformId;

import helios.core.types.StrongId;

using namespace helios::core::types;
export namespace helios::platform::runtime::types {

    /**
     * @brief Tag type used to specialize `StrongId` for runtime platform entities.
     */
    struct PlatformTag{};

    /**
     * @brief Strong id identifying runtime platform entities.
     */
    using PlatformId = StrongId<PlatformTag>;

}