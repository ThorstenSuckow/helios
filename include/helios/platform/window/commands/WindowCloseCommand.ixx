/**
 * @file WindowCloseCommand.ixx
 * @brief Command requesting closure of a specific window entity.
 */
module;

export module helios.platform.window.commands.WindowCloseCommand;

import helios.platform.window.types.WindowHandle;
import helios.platform.window.concepts.IsWindowHandle;

using namespace helios::platform::window::types;
using namespace helios::platform::window::concepts;
export namespace helios::platform::window::commands {

    /**
     * @brief Requests closing a single window.
     *
     * @tparam THandle Window handle type.
     */
    template<typename THandle>
    requires IsWindowHandle<THandle>
    struct WindowCloseCommand {

        /** @brief Target window entity handle. */
        THandle windowHandle;

    };


}