/**
 * @file GLFWWindowUserPointer.ixx
 * @brief Typed payload passed through GLFW's opaque user-pointer mechanism.
 */
module;

export module helios.platform.glfw.types.GLFWWindowUserPointer;

import helios.ecs.types.EntityHandle;
import helios.runtime.world.EngineWorld;
import helios.platform.window.concepts.IsWindowHandle;
import helios.runtime.messaging.command.concepts;

using namespace helios::ecs::types;
using namespace helios::runtime::world;
using namespace helios::platform::window::concepts;
using namespace helios::runtime::messaging::command::concepts;
export namespace helios::platform::glfw::types {

    /**
     * @brief Callback payload allowing GLFW callbacks to resolve window entity and game world.
     *
     * @tparam THandle Window handle type.
     */
    template<typename THandle, typename TPlatformCommandBuffer>
    requires IsWindowHandle<THandle> && IsPlatformCommandBuffer<TPlatformCommandBuffer>
    struct GLFWWindowUserPointer {
        /** @brief Window entity handle associated with the native window. */
        THandle windowHandle;

        /** @brief Runtime platform world used by callbacks to enqueue or mutate state. */
        TPlatformCommandBuffer* platformCommandBuffer = nullptr;
    };

}
