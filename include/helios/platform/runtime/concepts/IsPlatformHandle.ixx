/**
 * @file IsPlatformHandle.ixx
 * @brief Concept constraining runtime platform handle types.
 */
module;

#include <concepts>

export module helios.platform.runtime.concepts.IsPlatformHandle;

import helios.platform.runtime.types.PlatformHandle;

using namespace helios::platform::runtime::types;
export namespace helios::platform::runtime::concepts {

    /**
     * @brief True when `T` is exactly `PlatformHandle`.
     */
    template<typename T>
    concept IsPlatformHandle = std::is_same_v<T, PlatformHandle>;

}