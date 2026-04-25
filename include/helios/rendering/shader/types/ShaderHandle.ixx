/**
 * @file ShaderHandle.ixx
 * @brief Entity-handle alias for shader resources.
 */
module;

export module helios.rendering.shader.types.ShaderHandle;

import helios.ecs.types;
import helios.rendering.shader.types.ShaderDomainTag;

export namespace helios::rendering::shader::types {

    /**
     * @brief Handle type used to reference shader entities.
     */
    using ShaderHandle = helios::ecs::types::EntityHandle<ShaderDomainTag>;

}
