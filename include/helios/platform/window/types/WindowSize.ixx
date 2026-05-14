/**
 * @file WindowSize.ixx
 * @brief Value type describing a window size in pixels.
 */
module;

export module helios.platform.window.types.WindowSize;

import helios.math;

using namespace helios::math;
export namespace helios::platform::window::types {

    /**
     * @brief Width/height pair used for window creation and resize operations.
     */
    using WindowSize = helios::math::vec2f;

} // namespace helios::platform::window::types
