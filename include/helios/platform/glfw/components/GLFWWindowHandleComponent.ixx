/**
 * @file GLFWWindowHandleComponent.ixx
 * @brief Component storing native GLFW window handles for window entities.
 */
module;


#include <GLFW/glfw3.h>

export module helios.platform.glfw.components.GLFWWindowHandleComponent;

import helios.platform.window.concepts.IsWindowHandle;

using namespace helios::platform::window::concepts;
export namespace helios::platform::glfw::components {

    /**
     * @brief Binds a window entity to its native `GLFWwindow*` handle.
     *
     * @tparam THandle Window handle type.
     */
    template<typename THandle>
    requires IsWindowHandle<THandle>
    struct GLFWWindowHandleComponent {

        /** @brief Native GLFW window pointer. */
        GLFWwindow* handle = nullptr;

    };

} // namespace helios::platform::glfw::components
