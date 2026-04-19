/**
 * @file ShaderEntity.ixx
 * @brief ECS facade alias for shader resource entities.
 */
module;

export module helios.rendering.shader.ShaderEntity;

import helios.rendering.shader.ShaderEntityManager;
import helios.ecs.Entity;

using namespace helios::ecs;
export namespace helios::rendering::shader {

    /**
     * @brief Entity facade bound to `ShaderEntityManager`.
     */
    using ShaderEntity = Entity<ShaderEntityManager>;

} // namespace helios::rendering::shader
