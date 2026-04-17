/**
 * @file GLFWWindowUserPointerComponent.ixx
 * @brief Component storing typed user-pointer payload for GLFW callbacks.
 */
module;



export module helios.platform.glfw.components.GLFWWindowUserPointerComponent;

import helios.engine.runtime.world.GameWorld;
import helios.platform.glfw.types.GLFWWindowUserPointer;
import helios.platform.window.concepts.IsWindowHandle;

using namespace helios::engine::runtime::world;
using namespace helios::platform::window::concepts;
using namespace helios::platform::glfw::types;
export namespace helios::platform::glfw::components {


    /**
     * @brief Associates a window entity with callback payload used by GLFW user-pointer API.
     *
     * @tparam THandle Window handle type.
     */
    template<typename THandle>
    requires IsWindowHandle<THandle>
    struct GLFWWindowUserPointerComponent {
        /** @brief Typed payload exposed to GLFW callbacks via `glfwSetWindowUserPointer`. */
        GLFWWindowUserPointer<THandle> userPointer;

    };

} // namespace helios::platform::glfw::components
