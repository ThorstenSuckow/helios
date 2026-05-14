/**
 * @file SceneMemberRenderContext.ixx
 * @brief Render extraction payload for a scene member.
 */
module;


export module helios.scene.types.SceneMemberRenderContext;

import helios.ecs.types.EntityHandle;
import helios.scene.types.SceneHandle;
import helios.rendering.mesh.types;
import helios.rendering.material.types;
import helios.rendering.shader.types;
import helios.rendering.viewport.types;
import helios.rendering.framebuffer.types;
import helios.math.types;

using namespace helios::ecs::types;
using namespace helios::rendering::viewport::types;
using namespace helios::rendering::framebuffer::types;
using namespace helios::rendering::mesh::types;
using namespace helios::rendering::material::types;
using namespace helios::rendering::shader::types;
using namespace helios::scene;

export namespace helios::scene::types {

    /**
     * @brief Immutable render context for one scene member in one viewport.
     *
     * @tparam THandle Scene-member handle type (for example game-object handle).
     */
    template<typename TMemberHandle>
    struct SceneMemberRenderContext {
        /**
         * @brief Handle of the scene member origin entity.
         */
        TMemberHandle memberHandle;

        /** @brief Target viewport for rendering. */
        FramebufferHandle framebufferHandle;

        /** @brief Target viewport for rendering. */
        ViewportHandle viewportHandle;

        /** @brief Owning scene handle. */
        SceneHandle sceneHandle;

        /** @brief Mesh resource handle to render. */
        MeshHandle meshHandle;

        /** @brief Material resource handle to apply. */
        MaterialHandle materialHandle;

        /** @brief Shader resource handle used for draw submission. */
        ShaderHandle shaderHandle;

        /** @brief World transform used for rendering the scene member. */
        helios::math::mat4f worldTransform;
    };

}