/**
 * @file IsAnyPlatformHandle.ixx
 * @brief Concept accepting all platform-domain handle types.
 */
module;

#include <concepts>

export module helios.platform.concepts.IsAnyPlatformHandle;

import helios.platform.environment.concepts;
import helios.platform.window.concepts;

using namespace helios::platform::environment::concepts;
using namespace helios::platform::window::concepts;
export namespace helios::platform::concepts {

    /**
     * @brief True when `THandle` is either a runtime platform handle or a window handle.
     */
    template<typename THandle>
    concept IsAnyPlatformHandle = IsPlatformHandle<THandle> || IsWindowHandle<THandle>;

}
