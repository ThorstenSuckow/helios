/**
 * @file RenderCommandBuffer.ixx
 * @brief Concrete command buffer pre-configured with render related engine types.
 */
module;

#include <utility>
#include <type_traits>

export module helios.runtime.messaging.command.RenderCommandBuffer;


import helios.runtime.messaging.command.TypedCommandBuffer;

import helios.engine.modules.rendering.commands;
import helios.rendering.shader.commands;
import helios.rendering.shader.types;
import helios.runtime.world.types;
import helios.engine.common.tags;


using namespace helios::rendering::shader::types;
using namespace helios::runtime::world;
using namespace helios::runtime::world::types;
export namespace helios::runtime::messaging::command {

    using RenderCommandBuffer  = helios::runtime::messaging::command::TypedCommandBuffer<
        helios::engine::modules::rendering::commands::RenderCommand<GameObjectHandle>,
        helios::rendering::shader::commands::ShaderCompileCommand<ShaderHandle>,
        helios::rendering::shader::commands::ShaderBatchCompileCommand<ShaderHandle>
    >;
;


}

