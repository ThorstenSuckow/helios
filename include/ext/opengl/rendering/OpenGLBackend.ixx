module;

#include <memory>

export module helios.ext.opengl.rendering.OpenGLBackend;

import helios.math.types;


import helios.rendering.mesh;
import helios.rendering.shader;
import helios.rendering.material;
import helios.rendering.framebuffer;
import helios.rendering.viewport;
import helios.util.Colors;

import helios.scene.types.SceneMemberRenderContext;

import helios.engine.runtime.world.EngineWorld;

using namespace helios::math;
using namespace helios::rendering;
using namespace helios::rendering::shader::types;
using namespace helios::rendering::material::types;
using namespace helios::rendering::mesh::types;
using namespace helios::rendering::framebuffer::types;
using namespace helios::rendering::viewport::types;
using namespace helios::scene::types;
using namespace helios::engine::runtime::world;
export namespace helios::ext::opengl::rendering {


    class OpenGLBackend {

    private:

        RenderResourceWorld& renderResourcesWorld_;

        RenderTargetWorld& renderTargetsWorld_;

    public:



        explicit OpenGLBackend(
            RenderResourceWorld& renderResourcesWorld,
            RenderTargetWorld& renderTargetsWorld
        ) :
        renderResourcesWorld_(renderResourcesWorld),
        renderTargetsWorld_(renderTargetsWorld)
        {}


        void beginRenderPass(
            const FramebufferHandle framebufferHandle,
            const ViewportHandle viewportHandle,
            const vec4f clearColor) {


        }

        template<typename THandle>
        void doRender(const SceneMemberRenderContext<THandle>& renderContext)  {

        }

        void endRenderPass() {

        }

        void init() {

        }

        bool initialized() {
            return true;
        }




    };
} // namespace helios::ext::opengl::rendering
