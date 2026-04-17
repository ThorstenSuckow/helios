/**
 * @file WindowComponent.ixx
 * @brief Runtime component storing window metadata and geometry state.
 */
module;

#include <string>

export module helios.platform.window.components.WindowComponent;

import helios.platform.window.types.WindowSize;
import helios.platform.window.concepts.IsWindowHandle;

using namespace helios::platform::window::types;
using namespace helios::platform::window::concepts;
export namespace helios::platform::window::components {

    /**
     * @brief Stores platform-window properties associated with an entity.
     *
     * @tparam THandle Window handle type.
     */
    template<typename THandle>
    requires IsWindowHandle<THandle>
    struct WindowComponent {

        /** @brief Runtime title used by the window backend. */
        std::string title;

        /** @brief Current window size. */
        WindowSize size;

        /** @brief Aspect-ratio numerator used for ratio constraints. */
        float aspectRatioNumer;

        /** @brief Aspect-ratio denominator used for ratio constraints. */
        float aspectRatioDenom;

    };

} // namespace helios::platform::window::components
