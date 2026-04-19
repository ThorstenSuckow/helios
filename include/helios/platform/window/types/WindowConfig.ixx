/**
 * @file WindowConfig.ixx
 * @brief Configuration payload used to create a window.
 */
module;

#include <string>

export module helios.platform.window.types.WindowConfig;

import helios.platform.window.types.WindowSize;

export namespace helios::platform::window::types {

    /**
     * @brief Immutable-like value object carrying window creation parameters.
     */
    struct WindowConfig {

        /** @brief Window title shown by the platform window. */
        std::string title;

        /** @brief Initial window size. */
        WindowSize size;

        /** @brief Aspect-ratio numerator used for optional ratio locking. */
        float aspectRatioNumer;

        /** @brief Aspect-ratio denominator used for optional ratio locking. */
        float aspectRatioDenom;

    };

} // namespace helios::platform::window::types
