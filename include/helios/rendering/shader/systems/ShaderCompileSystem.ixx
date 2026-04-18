/**
 * @file ShaderCompileSystem.ixx
 * @brief Collects active shader resources and queues batch compile commands.
 */
module;

#include <span>
#include <vector>
#include "helios/helios_config.h"

export module helios.rendering.shader.systems.ShaderCompileSystem;

import helios.runtime.messaging.command.RenderCommandBuffer;
import helios.runtime.messaging.command.NullCommandBuffer;

import helios.runtime.world.UpdateContext;
import helios.runtime.world.tags.SystemRole;

import helios.rendering.shader.components;
import helios.rendering.shader.concepts;
import helios.rendering.shader.commands;

import helios.runtime.messaging.command.concepts.IsCommandBufferLike;

import helios.ecs.components;

using namespace helios::runtime::tags;
using namespace helios::runtime::world;
using namespace helios::runtime::messaging::command;
using namespace helios::rendering::shader;
using namespace helios::runtime::concepts;
using namespace helios::rendering::shader::components;
using namespace helios::rendering::shader::commands;
using namespace helios::ecs::components;
using namespace helios::rendering::shader::concepts;
export namespace helios::rendering::shader::systems {

    /**
     * @brief System that batches shader compile requests for active shader entities.
     *
     * @tparam THandle Shader handle type.
     * @tparam TCommandBuffer Command buffer type used for queued compile commands.
     * @tparam TCapacity Initial reserve size for the internal handle cache.
     */
    template<typename THandle, typename TCommandBuffer = NullCommandBuffer, size_t TCapacity = DEFAULT_SHADER_POOL_CAPACITY>
    requires IsShaderHandle<THandle> && IsCommandBufferLike<TCommandBuffer>
    class ShaderCompileSystem {

        std::vector<THandle> shaderHandles_;

        size_t capacity_;

    public:

        using EngineRoleTag = SystemRole;

        explicit ShaderCompileSystem(size_t capacity = TCapacity) : capacity_(capacity) {
            shaderHandles_.reserve(capacity);
        }

        /**
         * @brief Collects active shader handles and queues one batch compile command.
         *
         * @param updateContext Frame update context.
         */
        void update(UpdateContext& updateContext) noexcept {

            for (auto [entity, scc, ac] : updateContext.view<
                THandle,
                ShaderSourceComponent<THandle>,
                Active<THandle>
            >().whereEnabled()) {
                shaderHandles_.push_back(entity.handle());
            }

            updateContext.queueCommand<
                TCommandBuffer,
                ShaderBatchCompileCommand<THandle>>(std::move(shaderHandles_)
            );

            shaderHandles_.clear();
            shaderHandles_.reserve(capacity_);


        }

    };



}