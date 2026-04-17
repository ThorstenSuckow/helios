/**
 * @file GLFWWindowUserPointer.ixx
 * @brief Typed payload passed through GLFW's opaque user-pointer mechanism.
 */
module;

export module helios.platform.glfw.types.GLFWWindowUserPointer;

import helios.ecs.types.EntityHandle;
import helios.engine.runtime.world.GameWorld;
import helios.platform.window.concepts.IsWindowHandle;

using namespace helios::ecs::types;
using namespace helios::engine::runtime::world;
using namespace helios::platform::window::concepts;
export namespace helios::platform::glfw::types {

    /**
     * @brief Callback payload allowing GLFW callbacks to resolve window entity and game world.
     *
     * @tparam THandle Window handle type.
     */
    template<typename THandle>
    requires IsWindowHandle<THandle>
    struct GLFWWindowUserPointer {
        /** @brief Window entity handle associated with the native window. */
        THandle windowHandle;

        /** @brief Runtime world used by callbacks to enqueue or mutate state. */
        GameWorld* gameWorld = nullptr;
    };

}
