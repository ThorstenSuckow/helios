/**
 * @file WindowResizeCommand.ixx
 * @brief Command payload for window resize updates.
 */
module;

export module helios.platform.window.commands.WindowResizeCommand;

import helios.platform.window.types.WindowHandle;
import helios.platform.window.types.WindowSize;
import helios.platform.window.concepts.IsWindowHandle;

using namespace helios::platform::window::types;
using namespace helios::platform::window::concepts;
export namespace helios::platform::window::commands {

    /**
     * @brief Carries a resize update for a target window entity.
     *
     * @tparam THandle Window handle type.
     */
    template<typename THandle>
    requires IsWindowHandle<THandle>
    struct WindowResizeCommand {

        /** @brief Target window entity handle. */
        THandle windowHandle;

        /** @brief New window size in pixels. */
        WindowSize size;

    };



}