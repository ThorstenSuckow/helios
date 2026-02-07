/**
 * @file EmitterContext.ixx
 * @brief Context data from the entity that triggered a spawn.
 */
module;

export module helios.engine.runtime.spawn.EmitterContext;

import helios.math;
import helios.util;

import helios.engine.ecs.EntityHandle;

export namespace helios::engine::runtime::spawn {

    /**
     * @brief Context data from the entity that triggered a spawn operation.
     *
     * @details EmitterContext captures the state of the emitting entity at the
     * moment a spawn is requested. This allows spawned entities to inherit
     * properties from their source (e.g., projectiles inheriting velocity).
     *
     * Example usage:
     * ```cpp
     * EmitterContext ctx{
     *     .position = spaceship->position(),
     *     .velocity = spaceship->velocity()
     * };
     * spawnContext.emitterContext = ctx;
     * ```
     *
     * @see SpawnContext
     * @see SpawnCommand
     */
    struct EmitterContext {

        /**
         * @brief World position of the emitting entity.
         */
        const helios::math::vec3f position;

        /**
         * @brief Velocity of the emitting entity at spawn time.
         */
        const helios::math::vec3f velocity;

        /**
         * @brief Handle of the entity that is responsible for this EmitterContext.
         */
        const helios::engine::ecs::EntityHandle source;

    };


}