/**
 * @file ShaderId.ixx
 * @brief Strongly-typed identifier for shader resources.
 */
module;

export module helios.rendering.shader.types.ShaderId;

import helios.rendering.shader.types.ShaderDomainTag;
import helios.core.types.StrongId;

export namespace helios::rendering::shader::types {

    /**
     * @brief Strongly-typed identifier for shader resources.
     */
    using ShaderId = helios::core::types::StrongId<ShaderDomainTag>;

}
