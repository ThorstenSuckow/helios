/**
 * @file OpenGLFramebufferIdComponent.ixx
 * @brief OpenGL framebuffer ID component alias.
 */
module;

#include <cstdint>

export module helios.platform.opengl.components.OpenGLFramebufferIdComponent;

import helios.core.components.NumericValueComponent;

using namespace helios::core::components;
export namespace helios::platform::opengl::components {

    /** @brief Domain tag for OpenGL framebuffer object identifiers. */
    struct OpenGLFramebufferIdDomain {};

    /**
     * @brief Stores an OpenGL framebuffer object ID.
     *
     * @tparam THandle Owning entity handle type.
     */
    template<typename THandle>
    using OpenGLFramebufferIdComponent = NumericValueComponent<OpenGLFramebufferIdDomain, THandle, uint32_t>;

}