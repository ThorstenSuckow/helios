/**
 * @file SwapBuffersCommand.ixx
 * @brief Command requesting a back/front buffer swap for a window.
 */
module;


export module helios.platform.window.commands.SwapBuffersCommand;

import helios.platform.window.types.WindowHandle;
import helios.platform.window.concepts;

using namespace helios::platform::window::types;
using namespace helios::platform::window::concepts;
export namespace helios::platform::window::commands {

    /**
     * @brief Requests buffer swap for the given window handle.
     *
     * @tparam THandle Window handle type.
     */
    template<typename THandle>
    requires IsWindowHandle<THandle>
    struct SwapBuffersCommand {

        /** @brief Target window entity handle. */
        THandle windowHandle;

    };


}