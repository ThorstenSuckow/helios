/**
 * @file GLFWWindowUserPointerComponent.ixx
 * @brief Component storing typed user-pointer payload for GLFW callbacks.
 */
module;



export module helios.platform.glfw.components.GLFWWindowUserPointerComponent;

import helios.runtime.world.GameWorld;
import helios.platform.glfw.types.GLFWWindowUserPointer;
import helios.platform.window.concepts.IsWindowHandle;
import helios.runtime.messaging.command.concepts.IsPlatformCommandBuffer;

using namespace helios::runtime::world;
using namespace helios::platform::window::concepts;
using namespace helios::platform::glfw::types;
using namespace helios::runtime::messaging::command::concepts;
export namespace helios::platform::glfw::components {


    /**
     * @brief Associates a window entity with callback payload used by GLFW user-pointer API.
     *
     * @tparam THandle Window handle type.
     */
    template<typename THandle, typename TPlatformCommandBuffer>
    requires IsWindowHandle<THandle> && IsPlatformCommandBuffer<TPlatformCommandBuffer>
    struct GLFWWindowUserPointerComponent {
        /** @brief Typed payload exposed to GLFW callbacks via `glfwSetWindowUserPointer`. */
        GLFWWindowUserPointer<THandle, TPlatformCommandBuffer> userPointer;

    };

} // namespace helios::platform::glfw::components
