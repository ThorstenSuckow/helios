/**
 * @file ShaderEntityManager.ixx
 * @brief Shader-domain ECS registry and manager aliases.
 */
module;

#include "helios/helios_config.h"

export module helios.rendering.shader.ShaderEntityManager;

import helios.ecs.EntityManager;
import helios.ecs.EntityRegistry;
import helios.ecs.strategies;

import helios.rendering.shader.types;

constexpr auto SHADER_SIZE = DEFAULT_SHADER_POOL_CAPACITY;

using namespace helios::rendering::shader::types;
using namespace helios::ecs;
using namespace helios::ecs::strategies;
export namespace helios::rendering::shader {

    /**
     * @brief Entity registry used for shader resources.
     */
    using ShaderEntityRegistry = EntityRegistry<ShaderId, HashedLookupStrategy, true, SHADER_SIZE>;

    /**
     * @brief Entity manager used for shader handles and shader components.
     */
    using ShaderEntityManager = EntityManager<ShaderHandle, ShaderEntityRegistry, SHADER_SIZE>;


}