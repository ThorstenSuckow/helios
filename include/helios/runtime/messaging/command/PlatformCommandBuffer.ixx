/**
 * @file PlatformCommandBuffer.ixx
 * @brief Typed command buffer for platform/window command dispatch.
 */
module;

export module helios.runtime.messaging.command.PlatformCommandBuffer;



import helios.platform.environment.types;
import helios.platform.environment.commands;
import helios.platform.window.commands;

import helios.platform.lifecycle.commands;

import helios.runtime.messaging.command.TypedCommandBuffer;

import helios.platform.window.types.WindowHandle;

using namespace helios::platform::environment::types;
using namespace helios::platform::window::types;
export namespace helios::runtime::messaging::command {

    /**
     * @brief Platform-focused `TypedCommandBuffer` specialization.
     *
     * @details Bundles window lifecycle commands (create/resize/swap/close)
     * and runtime platform commands (init/poll/shutdown) into a single
     * concrete command buffer type.
     */
    using PlatformCommandBuffer = helios::runtime::messaging::command::TypedCommandBuffer<

            // window
            helios::platform::window::commands::WindowCreateCommand<WindowHandle>,
            helios::platform::window::commands::WindowResizeCommand<WindowHandle>,
            helios::platform::window::commands::SwapBuffersCommand<WindowHandle>,
            helios::platform::window::commands::WindowCloseCommand<WindowHandle>,

            // runtime platform
            helios::platform::lifecycle::commands::PlatformInitCommand,
            helios::platform::environment::commands::PollEventsCommand,
            helios::platform::lifecycle::commands::ShutdownCommand
        >;

}

