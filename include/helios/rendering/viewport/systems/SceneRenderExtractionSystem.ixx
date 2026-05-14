/**
 * @file SceneRenderExtractionSystem.ixx
 * @brief Extracts visible scene members into render commands per viewport.
 */
module;

#include <concepts>
#include <cassert>

export module helios.rendering.viewport.systems.SceneRenderExtractionSystem;

import helios.rendering.viewport.concepts.IsViewportHandle;

import helios.scene.types;
import helios.scene.components;
import helios.scene.concepts.IsFrustumCullerLike;

import helios.rendering.common.components;
import helios.rendering.common.commands.RenderCommand;

import helios.rendering.framebuffer.components.FramebufferBindingComponent;


import helios.runtime.world.UpdateContext;
import helios.runtime.messaging.command.NullCommandBuffer;
import helios.runtime.messaging.command.concepts.IsCommandBufferLike;
import helios.runtime.world.tags.SystemRole;

import helios.ecs.components.Active;

import helios.spatial.components.LocalToWorldBoundsComponent;
import helios.spatial.components.LocalToWorldMatrixComponent;

import helios.util.log;

using namespace helios::scene;
using namespace helios::scene::types;
using namespace helios::scene::concepts;
using namespace helios::scene::components;
using namespace helios::ecs::components;
using namespace helios::rendering::common::components;
using namespace helios::rendering::viewport::concepts;
using namespace helios::rendering::framebuffer::components;
using namespace helios::scene::types;
using namespace helios::runtime::messaging::command::concepts;
using namespace helios::spatial::components;
using namespace helios::rendering::common::commands;
using namespace helios::rendering::common::components;
using namespace helios::runtime::messaging::command;

#define HELIOS_LOG_SCOPE "helios::rendering::viewport::systems::SceneRenderExtractionSystem"
export namespace helios::rendering::viewport::systems {

    /**
     * @brief System that extracts per-member render contexts for active viewports.
     *
     * @details For each active viewport binding, the system resolves the bound
     * scene and camera, evaluates the configured culling strategy for scene
     * members, and writes accepted members as `RenderCommand`s into the target
     * command buffer.
     *
     * @tparam TOwnerHandle Viewport entity handle type.
     * @tparam TMemberHandle Scene member handle type.
     * @tparam TCullingStrategy Frustum-culling strategy type.
     * @tparam TCommandBuffer Command buffer used for extracted render commands.
     */
    template<
        typename TOwnerHandle,
        typename TMemberHandle,
        typename TCullingStrategy,
        typename TCommandBuffer = NullCommandBuffer
    >
    requires IsViewportHandle<TOwnerHandle> &&
             IsFrustumCullerLike<TCullingStrategy, typename TCullingStrategy::MemberHandle_type> &&
             std::same_as<typename TCullingStrategy::MemberHandle_type, TMemberHandle> &&
             IsCommandBufferLike<TCommandBuffer>
    class SceneRenderExtractionSystem {

        TCullingStrategy cullingStrategy_;

        static inline auto& logger_ = helios::util::log::LogManager::loggerForScope(HELIOS_LOG_SCOPE);

    public:

        /** @brief Runtime role tag used for engine system registration. */
        using EngineRoleTag = helios::runtime::world::tags::SystemRole;

        /** @brief Command buffer type used by this extraction system. */
        using CommandBuffer_type = TCommandBuffer;

        /**
         * @brief Constructs the system with a culling strategy instance.
         *
         * @param cullingStrategy Strategy used to decide whether a scene member
         * should produce render work.
         */
        explicit SceneRenderExtractionSystem(TCullingStrategy cullingStrategy = TCullingStrategy())
        : cullingStrategy_(std::move(cullingStrategy)) {}

        /**
         * @brief Extracts render commands for active viewports.
         *
         * @param updateContext Current frame update context.
         * @param cmdBuffer Command buffer receiving extracted render commands.
         */
        void update(helios::runtime::world::UpdateContext& updateContext, TCommandBuffer& cmdBuffer) noexcept {


            for (auto [viewportEntity, fbc, sbc, cbc, viewportActive] : updateContext.view<
                TOwnerHandle,
                FramebufferBindingComponent<TOwnerHandle>,
                SceneBindingComponent<TOwnerHandle>,
                CameraBindingComponent<TOwnerHandle>,
                Active<TOwnerHandle>
            >().whereEnabled()) {

                const auto sceneHandle  = sbc->targetHandle();
                const auto cameraHandle = cbc->targetHandle();

                const auto camera = updateContext.find<TMemberHandle>(cameraHandle);
                if (!camera) {
                    assert(false && "Camera not found");
                    logger_.error("Camera not found");
                    continue;
                }
                auto* pcc = camera->template get<ProjectionMatrixComponent<TMemberHandle>>();
                if (!pcc) {
                    assert(pcc && "Camera had no PerspectiveCameraComponent");
                    logger_.error("Camera had no PerspectiveCameraComponent");
                    continue;
                }
                auto* lac = camera->template get<ViewMatrixComponent<TMemberHandle>>();
                if (!lac) {
                    assert(lac && "Camera had no ViewMatrixComponent");
                    logger_.error("Camera had no ViewMatrixComponent");
                    continue;
                }

                for (auto [
                    memberEntity,
                    smc,
                    rpc,
                    clwbc,
                    clwmc,
                    memberActive
                    ] : updateContext.view<
                    TMemberHandle,
                    SceneMemberComponent<TMemberHandle>,
                    RenderPrototypeComponent<TMemberHandle>,
                    LocalToWorldBoundsComponent<TMemberHandle>,
                    LocalToWorldMatrixComponent<TMemberHandle>,
                    Active<TMemberHandle>
                >().whereEnabled()) {

                    if (smc->targetHandle() == sceneHandle && cullingStrategy_.shouldRender(
                            CullingContext{pcc->value(), lac->value(), clwbc->value(), memberEntity.handle()}
                        )
                    ) {

                        cmdBuffer.template add<RenderCommand<TMemberHandle>>(
                            SceneMemberRenderContext<TMemberHandle>{
                                memberEntity.handle(),
                                fbc->targetHandle(),
                                viewportEntity.handle(),
                                sceneHandle,
                                rpc->meshHandle(),
                                rpc->materialHandle(),
                                rpc->shaderHandle(),
                                clwmc->value()
                        });


                    }
                }
            }

        }

    };

}