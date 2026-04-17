/**
 * @file WindowResizeCommand.ixx
 * @brief Command payload for window resize updates.
 */
module;

export module helios.platform.window.commands.WindowResizeCommand;

import helios.platform.window.types.WindowHandle;
import helios.platform.window.types.WindowSize;

using namespace helios::platform::window::types;
export namespace helios::platform::window::commands {

    /**
     * @brief Carries a resize update for a target window entity.
     *
     * @tparam THandle Window handle type.
     */
    template<typename THandle>
    struct WindowResizeCommand {

        /** @brief Target window entity handle. */
        WindowHandle windowHandle;

        /** @brief New window size in pixels. */
        WindowSize size;

    };



}