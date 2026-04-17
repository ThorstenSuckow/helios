/**
 * @file WindowSize.ixx
 * @brief Value type describing a window size in pixels.
 */
module;

export module helios.platform.window.types.WindowSize;

export namespace helios::platform::window::types {

    /**
     * @brief Width/height pair used for window creation and resize operations.
     */
    struct WindowSize {

        /** @brief Window width in pixels. */
        unsigned int width;

        /** @brief Window height in pixels. */
        unsigned int height;

    };

} // namespace helios::platform::window::types
