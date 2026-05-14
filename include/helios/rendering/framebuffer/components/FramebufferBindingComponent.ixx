/**
 * @file FramebufferBindingComponent.ixx
 * @brief Binding component alias for framebuffer targets.
 */
module;


export module helios.rendering.framebuffer.components.FramebufferBindingComponent;

import helios.rendering.framebuffer.types.FramebufferHandle;
import helios.ecs.components.BindingComponent;

using namespace helios::rendering::framebuffer::types;
using namespace helios::ecs::components;
export namespace helios::rendering::framebuffer::components {

    /**
     * @brief Binds an owning entity to a framebuffer handle.
     *
     * @tparam TOwnerHandle Owning entity handle type.
     */
    template<typename TOwnerHandle>
    using FramebufferBindingComponent = BindingComponent<TOwnerHandle, FramebufferHandle>;

}