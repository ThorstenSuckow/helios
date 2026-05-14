/**
 * @file WindowSizeUpdateSystem.ixx
 * @brief Updates framebuffer-related state after window size changes.
 */
module;

#include <cassert>

export module helios.platform.window.systems.WindowSizeUpdateSystem;

import helios.runtime.world.tags.SystemRole;
import helios.runtime.world.UpdateContext;

import helios.platform.window.components;
import helios.ecs.components.Active;
import helios.platform.window.concepts.IsWindowHandle;

import helios.rendering.framebuffer;

import helios.util.log;

import helios.spatial.components.Size2DComponent;

using namespace helios::spatial::components;
using namespace helios::rendering::framebuffer::components;
using namespace helios::rendering::framebuffer::types;
using namespace helios::runtime::world::tags;
using namespace helios::platform::window::concepts;
using namespace helios::runtime::world;
using namespace helios::platform::window::components;
using namespace helios::ecs::components;

#define HELIOS_LOG_SCOPE "helios::platform::window::systems::WindowSizeUpdateSystem"
export namespace helios::platform::window::systems {

    /**
     * @brief System that reacts to dirty window size components.
     *
     * @tparam TMemberHandle Window entity handle type.
     */
    template<typename TMemberHandle>
    requires IsWindowHandle<TMemberHandle>
    class WindowSizeUpdateSystem {

        static inline auto& logger_ = helios::util::log::LogManager::loggerForScope(HELIOS_LOG_SCOPE);

    public:

        /** @brief Runtime role tag used for engine system registration. */
        using EngineRoleTag = SystemRole;

        /**
         * @brief Processes active windows with dirty size state.
         *
         * @param updateContext Frame update context.
         */
        void update(UpdateContext& updateContext) noexcept {
            for (auto [entity, wc, wsc, fbc, active] : updateContext.view<
                TMemberHandle,
                WindowComponent<TMemberHandle>,
                Size2DComponent<TMemberHandle>,
                FramebufferBindingComponent<TMemberHandle>,
                Active<TMemberHandle>
            >().whereEnabled()) {
                if (wsc->isDirty()) {
                    // c'mon now do something
                }
            }
        };

    };

}