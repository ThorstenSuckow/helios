/**
 * @file OpenGLBackend.ixx
 * @brief Draft OpenGL backend for render pass execution.
 */
module;

#include <glad/gl.h>
#include <memory>
#include <cassert>
#include <utility>
#include "helios/helios_config.h"


export module helios.ext.opengl.rendering.OpenGLBackend;

import helios.math.types;

import helios.util.log;

import helios.rendering.common.types;
import helios.rendering.common.components;

import helios.spatial.components;

import helios.core.components;

import helios.platform.opengl;

import helios.rendering.mesh;
import helios.rendering.shader;
import helios.rendering.material;
import helios.rendering.framebuffer;
import helios.rendering.viewport;
import helios.util.Colors;

import helios.scene.types.SceneMemberRenderContext;

import helios.runtime.world.EngineWorld;

using namespace helios::core::components;
using namespace helios::math;
using namespace helios::rendering;
using namespace helios::rendering::shader::types;
using namespace helios::platform::opengl::components;
using namespace helios::spatial::components;
using namespace helios::rendering::material::types;
using namespace helios::rendering::common::types;
using namespace helios::rendering::common::components;
using namespace helios::rendering::mesh::types;
using namespace helios::rendering::framebuffer::types;
using namespace helios::rendering::viewport::types;
using namespace helios::scene::types;
using namespace helios::runtime::world;
using namespace helios::util::log;

#define HELIOS_LOG_SCOPE "helios::ext::opengl::rendering"
export namespace helios::ext::opengl::rendering {


    /**
     * @brief Draft backend that applies render pass state through OpenGL.
     */
    class OpenGLBackend {

    private:

        RenderResourceWorld& renderResourcesWorld_;

        RenderTargetWorld& renderTargetsWorld_;

        inline static const helios::util::log::Logger& logger_ = helios::util::log::LogManager::loggerForScope(
            HELIOS_LOG_SCOPE
        );

    public:



        /**
         * @brief Constructs the backend from render resource and target worlds.
         *
         * @param renderResourcesWorld Render resource world.
         * @param renderTargetsWorld Render target world.
         */
        explicit OpenGLBackend(
            RenderResourceWorld& renderResourcesWorld,
            RenderTargetWorld& renderTargetsWorld
        ) :
        renderResourcesWorld_(renderResourcesWorld),
        renderTargetsWorld_(renderTargetsWorld)
        {}


        /**
         * @brief Begins a render pass and configures the active framebuffer and viewport.
         *
         * @param renderPassContext Render pass target context.
         */
        void beginRenderPass(const RenderPassContext& renderPassContext) {

            auto viewportHandle = renderPassContext.viewportHandle;
            auto framebufferHandle = renderPassContext.framebufferHandle;

            auto viewport = renderTargetsWorld_.findEntity<ViewportHandle>(viewportHandle);
            auto framebuffer = renderTargetsWorld_.findEntity<FramebufferHandle>(framebufferHandle);

            #ifdef HELIOS_DEBUG
            if (!viewport) {
                logger_.error("Missing Viewport for handle {0}.", viewportHandle.entityId);
                assert(viewport && "Missing Viewport for handle.");
            }
            if (!framebuffer) {
                logger_.error("Missing Framebuffer for handle {0}.", framebufferHandle.entityId);
                assert(framebuffer && "Missing Framebuffer for handle.");
            }
            #endif

            const auto clearColor = framebuffer->get<ColorComponent<FramebufferHandle>>()->value();
            const auto clearFlags = std::to_underlying(framebuffer->get<ClearComponent<FramebufferHandle>>()->flags);
            const auto framebufferId = framebuffer->get<OpenGLFramebufferIdComponent<FramebufferHandle>>()->value();

            auto viewportBounds  = viewport->get<BoundsComponent<ViewportHandle>>()->value();
            auto framebufferSize = framebuffer->get<Size2DComponent<FramebufferHandle>>()->value();

            glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);

            #ifdef HELIOS_DEBUG
            const auto isValidFramebuffer = framebufferId == 0 ||
                (glIsFramebuffer(framebufferId) == GL_TRUE && glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
            if (!isValidFramebuffer) {
                logger_.error("Framebuffer with id {0} undefined.", framebufferId);
                assert(isValidFramebuffer && "Framebuffer id does not seem to be a valid id.");
            }
            #endif


            // this is equally important for the GlpyhTextRenderer
            // enable blending since the font's fragment shader uses the alpha channel
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);

            const auto x = static_cast<int>(framebufferSize[0] * viewportBounds[0]);
            const auto y = static_cast<int>(framebufferSize[1] * viewportBounds[1]);
            const auto width = static_cast<int>(framebufferSize[0] * viewportBounds[2]);
            const auto height = static_cast<int>(framebufferSize[1] * viewportBounds[3]);

            const auto clearMask = ((clearFlags & std::to_underlying(ClearFlags::Color)) ? GL_COLOR_BUFFER_BIT : 0) |
                ((clearFlags & std::to_underlying(ClearFlags::Depth)) ? GL_DEPTH_BUFFER_BIT : 0) |
                ((clearFlags & std::to_underlying(ClearFlags::Stencil)) ? GL_STENCIL_BUFFER_BIT : 0);

            glViewport(x, y, width, height);
            glScissor(x, y, width, height);
            glEnable(GL_SCISSOR_TEST);
            if (clearMask != 0) {
                glClear(clearMask);
            }


        }

        /**
         * @brief Renders one extracted scene member.
         *
         * @tparam THandle Scene member handle type.
         * @param renderContext Per-member render context.
         */
        template<typename THandle>
        void doRender(const SceneMemberRenderContext<THandle>& renderContext)  {

        }

        /**
         * @brief Ends the current render pass.
         *
         * @param renderPassContext Render pass target context.
         */
        void endRenderPass(const RenderPassContext& renderPassContext) {

            glDisable(GL_SCISSOR_TEST);

        }


    };
} // namespace helios::ext::opengl::rendering
