/**
 * @file WindowCreateCommand.ixx
 * @brief Command payload for creating a platform window.
 */
module;

#include <string>

export module helios.platform.window.commands.WindowCreateCommand;

import helios.platform.window.types.WindowConfig;

import helios.platform.window.types.WindowHandle;
import helios.platform.window.concepts.IsWindowHandle;

using namespace helios::platform::window::types;
using namespace helios::platform::window::concepts;
export namespace helios::platform::window::commands {

    /**
     * @brief Requests creation of a window for a specific window entity.
     *
     * @tparam THandle Window handle type.
     */
    template<typename THandle>
    requires IsWindowHandle<THandle>
    struct WindowCreateCommand {
        /** @brief Target window entity handle. */
        THandle windowHandle;

        /** @brief Creation configuration payload. */
        WindowConfig windowConfig;
    };


}