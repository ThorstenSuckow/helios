/**
 * @file RenderManager.ixx
 * @brief Draft render manager that batches render commands per scene and target.
 */
module;

#include <vector>
#include <format>
#include "helios/helios_config.h"
#include <optional>

export module helios.rendering.RenderManager;

import helios.runtime.world.tags.ManagerRole;

import helios.rendering.framebuffer.types.FramebufferHandle;
import helios.rendering.viewport.types.ViewportHandle;
import helios.scene.types.SceneHandle;

import helios.rendering.common.commands.RenderCommand;
import helios.rendering.common.types.RenderPassContext;
import helios.scene.types.SceneMemberRenderContext;
import helios.runtime.messaging.command.CommandHandlerRegistry;

import helios.ecs.types.TypeDefs;

import helios.util.log;
import helios.runtime.world.UpdateContext;

import helios.rendering.common.concepts.IsRenderBackendLike;

import helios.core.container.HandleMultiMap;

using namespace helios::core::container;
using namespace helios::runtime::world;
using namespace helios::runtime::world::tags;
using namespace helios::runtime::messaging::command;
using namespace helios::rendering::common::commands;
using namespace helios::rendering::common::types;

using namespace helios::rendering::framebuffer::types;
using namespace helios::rendering::viewport::types;
using namespace helios::scene::types;

using namespace helios::scene::types;
using namespace helios::util::log;
using namespace helios::rendering::common::concepts;

#define HELIOS_LOG_SCOPE "helios::rendering::RenderManager"
export namespace helios::rendering {


    /**
     * @brief Draft manager that collects render commands and drives render passes.
     *
     * @tparam TRenderBackend Rendering backend type.
     * @tparam TMemberHandle Renderable scene member handle type.
     */
    template<typename TRenderBackend, typename TMemberHandle>
    requires IsRenderBackendLike<TRenderBackend, TMemberHandle>
    class RenderManager {


        inline static auto& logger_ = LogManager::loggerForScope(HELIOS_LOG_SCOPE);


        HandleMultiMap<FramebufferHandle, ViewportHandle> framebufferToViewport_;
        HandleMultiMap<SceneHandle, ViewportHandle> sceneToViewport_;
        std::vector<std::vector<SceneMemberRenderContext<TMemberHandle>>> sceneMemberRenderContexts_;

        TRenderBackend& renderBackend_;

    public:

        /** @brief Runtime role tag used for engine manager registration. */
        using EngineRoleTag = ManagerRole;

        /**
         * @brief Constructs the manager for a specific render backend.
         *
         * @param renderBackend Backend used to execute render passes.
         */
        explicit RenderManager(TRenderBackend& renderBackend) : renderBackend_(renderBackend) {

            framebufferToViewport_.reserve(DEFAULT_FRAMEBUFFER_POOL_CAPACITY, DEFAULT_VIEWPORT_POOL_CAPACITY);
            sceneToViewport_.reserve(DEFAULT_SCENE_POOL_CAPACITY, DEFAULT_VIEWPORT_POOL_CAPACITY);
            sceneMemberRenderContexts_.reserve(DEFAULT_SCENE_POOL_CAPACITY);

        }


        /**
         * @brief Flushes queued render work to the backend.
         *
         * @param updateContext Current frame update context.
         */
        void flush(UpdateContext& updateContext) {


            for (auto [framebufferHandle, viewportHandle]: framebufferToViewport_) {

                    const auto renderPassContext = RenderPassContext{
                        framebufferHandle, viewportHandle
                    };
                    renderBackend_.beginRenderPass(renderPassContext);

                    auto sceneHandle = sceneToViewport_.key(viewportHandle);

                    auto& sceneMemberRenderContexts = sceneMemberRenderContexts_[sceneHandle.entityId];


                    for (auto renderContext : sceneMemberRenderContexts) {

                    }


                   renderBackend_.endRenderPass(renderPassContext);


            }

            framebufferToViewport_.clear();
            sceneToViewport_.clear();
            sceneMemberRenderContexts_.clear();
        }

        /**
         * @brief Queues a render command for later execution.
         *
         * @param renderCommand Command containing per-member render context.
         * @return `true` if the command was accepted.
         */
        bool submit(RenderCommand<TMemberHandle> renderCommand) noexcept {

            const auto& sceneMemberRenderContext = renderCommand.sceneMemberRenderContext;
            const auto sceneIdx = sceneMemberRenderContext.sceneHandle.entityId;

            framebufferToViewport_.bind(
                sceneMemberRenderContext.framebufferHandle,
                sceneMemberRenderContext.viewportHandle
            );

            sceneToViewport_.bind(
                sceneMemberRenderContext.sceneHandle,
                sceneMemberRenderContext.viewportHandle
            );

            if (sceneMemberRenderContexts_.size() <= sceneIdx) {
                sceneMemberRenderContexts_.resize(sceneIdx + 1);
            }

            sceneMemberRenderContexts_[sceneIdx].push_back(std::move(renderCommand.sceneMemberRenderContext));

            return true;
        }

        /**
         * @brief Registers this manager as a render command handler.
         *
         * @param commandHandlerRegistry Command handler registry used at runtime.
         */
        void init(CommandHandlerRegistry& commandHandlerRegistry) noexcept {

            commandHandlerRegistry.handleCommands<
                RenderCommand<TMemberHandle>
            >(*this);


        };


    };


}