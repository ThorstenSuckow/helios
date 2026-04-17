/**
 * @file ShaderId.ixx
 * @brief Strongly-typed identifier for shader resources.
 */
module;

export module helios.rendering.shader.types.ShaderId;

import helios.core.types.FuncDefs;
import helios.core.types;
import helios.ecs.types;

export namespace helios::rendering::shader::types {

    /**
     * @brief Tag type for ShaderId.
     */
    struct ShaderIdTag{};

    /**
     * @brief Strongly-typed identifier for shader resources.
     */
    using ShaderId = helios::core::types::StrongId<ShaderIdTag>;

}
