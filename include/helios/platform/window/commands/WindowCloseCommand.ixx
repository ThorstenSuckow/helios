/**
 * @file WindowCloseCommand.ixx
 * @brief Command requesting closure of a specific window entity.
 */
module;

export module helios.platform.window.commands.WindowCloseCommand;

import helios.platform.window.types.WindowHandle;

using namespace helios::platform::window::types;
export namespace helios::platform::window::commands {

    /**
     * @brief Requests closing a single window.
     *
     * @tparam THandle Window handle type.
     */
    template<typename THandle>
    struct WindowCloseCommand {

        /** @brief Target window entity handle. */
        WindowHandle windowHandle;

    };


}