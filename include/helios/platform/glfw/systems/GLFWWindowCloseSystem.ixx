/**
 * @file GLFWWindowCloseSystem.ixx
 * @brief System that converts native GLFW close requests into engine window-close commands.
 */
module;

#include <GLFW/glfw3.h>

export module helios.platform.glfw.systems.GLFWWindowCloseSystem;



import helios.engine.runtime.world.UpdateContext;

import helios.engine.common.tags.SystemRole;

import helios.platform.glfw.components;
import helios.platform.window.components;
import helios.platform.window.commands.WindowCloseCommand;
import helios.engine.runtime.messaging.command.EngineCommandBuffer;

import helios.ecs.components.Active;
import helios.platform.window.concepts.IsWindowHandle;

using namespace helios::engine::common::tags;
using namespace helios::engine::runtime::world;
using namespace helios::engine::runtime::messaging::command;
using namespace helios::platform::glfw::components;
using namespace helios::platform::window::commands;
using namespace helios::platform::window::components;
using namespace helios::ecs::components;
using namespace helios::platform::window::concepts;
export namespace helios::platform::glfw::systems {

    /**
     * @brief Emits `WindowCloseCommand` when GLFW reports a close request.
     *
     * @tparam THandle Window handle type.
     */
    template<typename THandle>
    requires IsWindowHandle<THandle>
    class GLFWWindowCloseSystem {


    public:

        /**
         * @brief Engine role marker used by runtime registries.
         */
        using EngineRoleTag = SystemRole;

        /**
         * @brief Scans shown windows and queues close commands for requested closures.
         *
         * @param updateContext Frame-local update context.
         */
        void update(UpdateContext& updateContext) noexcept {

            for (auto [entity, wc, glfw, wsc, active]: updateContext.view<
                THandle,
                WindowComponent<THandle>,
                GLFWWindowHandleComponent<THandle>,
                WindowShownComponent<THandle>,
                Active<THandle>
                >().whereEnabled()) {
                if (glfwWindowShouldClose(glfw->handle)) {
                    updateContext.queueCommand<EngineCommandBuffer, WindowCloseCommand<THandle>>(
                        entity.handle()
                    );
                }
            }
        }

    };

}