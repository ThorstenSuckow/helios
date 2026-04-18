module;


export module helios.scene.systems.SceneMemberRenderExtractionSystem;

import helios.rendering.viewport.types.ViewportHandle;

import helios.ecs.types.EntityHandle;

import helios.scene.types;

import helios.scene.types;
import helios.scene.components;

import helios.runtime.world.Session;

import helios.ecs.types.EntityHandle;


import helios.rendering.components;
import helios.rendering.commands;

import helios.runtime.world.GameWorld;
import helios.runtime.world.UpdateContext;
import helios.runtime.messaging.command.NullCommandBuffer;
import helios.engine.common.concepts.IsCommandBufferLike;

import helios.ecs.components.Active;

import helios.spatial.transform.components.WorldTransformStateComponent;

import helios.engine.common.tags.SystemRole;

import helios.engine.common.concepts.IsFrustumCullerLike;

import helios.rendering.commands.RenderCommand;

using namespace helios::ecs::types;

using namespace helios::scene;
using namespace helios::scene::types;
using namespace helios::scene::components;
using namespace helios::ecs::components;
using namespace helios::rendering::components;
using namespace helios::scene::types;
using namespace helios::engine::common::concepts;
using namespace helios::spatial::transform::components;
using namespace helios::rendering::commands;
using namespace helios::runtime::messaging::command;

export namespace helios::scene::systems {

    /**
     *
     * @todo implement culling
     *
     * @tparam CullingStrategy
     */
    template<typename THandle, typename CullingStrategy, typename TCommandBuffer = NullCommandBuffer>
    requires IsFrustumCullerLike<CullingStrategy> && IsCommandBufferLike<TCommandBuffer>
    class SceneMemberRenderExtractionSystem {

        CullingStrategy cullingStrategy_;

    public:

        using EngineRoleTag = helios::engine::common::tags::SystemRole;


        void update(helios::runtime::world::UpdateContext& updateContext) noexcept {


            for (auto [entity, vc, active] : updateContext.view<
                THandle,
                ViewportComponent<THandle>,
                Active<THandle>
            >().whereEnabled()) {

                const auto viewportHandle = vc->viewportHandle();

                if (!updateContext.session().isActiveViewport(viewportHandle)) {
                    continue;
                }

                const auto sceneHandle = vc->sceneHandle();

                for (auto [innerEntity, smc, rpc, wtsc, innerActive] : updateContext.view<
                    THandle,
                    SceneMemberComponent<THandle>,
                    RenderPrototypeComponent<THandle>,
                    WorldTransformStateComponent<THandle>,
                    Active<THandle>
                >().whereEnabled()) {
                    if (smc->sceneHandle() == sceneHandle) {

                        auto* mcOverride = innerEntity.get<MaterialOverrideComponent>();

                        updateContext.queueCommand<TCommandBuffer, RenderCommand>(
                            SceneMemberRenderContext{
                                innerEntity.handle(),
                                viewportHandle,
                                smc->sceneHandle(),
                                rpc->meshHandle(),
                                mcOverride ? mcOverride->materialHandle() : rpc->materialHandle(),
                                rpc->shaderHandle(),
                                wtsc->worldTransform()
                        });


                    }
                }
            }

        }

    };

}