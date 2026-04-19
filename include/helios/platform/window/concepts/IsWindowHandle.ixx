/**
 * @file IsWindowHandle.ixx
 * @brief Concept constraining window handle types.
 */
module;

#include <concepts>

export module helios.platform.window.concepts.IsWindowHandle;

import helios.platform.window.types.WindowHandle;

using namespace helios::platform::window::types;
export namespace helios::platform::window::concepts {

    /**
     * @brief True when `T` is exactly `WindowHandle`.
     */
    template<typename T>
    concept IsWindowHandle = std::is_same_v<T, WindowHandle>;

}