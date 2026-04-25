/**
 * @file PlatformId.ixx
 * @brief Strong-id type definition for runtime platform entities.
 */
module;

export module helios.platform.environment.types.PlatformId;

import helios.core.types.StrongId;
import helios.platform.environment.types.PlatformDomainTag;

using namespace helios::core::types;
export namespace helios::platform::environment::types {

    /**
     * @brief Strong id identifying runtime platform entities.
     */
    using PlatformId = StrongId<PlatformDomainTag>;

}