/**
 * @file ProjectileSpawnRequest.ixx
 * @brief Data structure for projectile spawn requests.
 */
module;

export module helios.engine.mechanics.combat.requests.ProjectileSpawnRequest;

import helios.math;

export namespace helios::engine::mechanics::combat::requests {

    /**
     * @brief Data transfer object for projectile spawn requests.
     *
     * @details ProjectileSpawnRequest contains all parameters needed to spawn
     * a projectile from a source entity. It is created by the ShootCommandDispatcher
     * and queued in the ProjectilePoolManager for deferred processing.
     *
     * The struct captures:
     * - Spawn position (from the source entity)
     * - Movement direction (from aim component)
     * - Throttle/speed multiplier
     * - Inherited velocity (for momentum inheritance)
     * - Fire intensity (affects spawn rate accumulation)
     *
     * @see ShootCommandDispatcher
     * @see ProjectilePoolManager
     */
    struct ProjectileSpawnRequest {

        /**
         * @brief World-space spawn position for the projectile.
         */
        const helios::math::vec3f location;

        /**
         * @brief Normalized direction vector for projectile movement.
         */
        const helios::math::vec3f direction;

        /**
         * @brief Throttle multiplier for projectile speed (0.0 to 1.0).
         */
        const float throttle;

        /**
         * @brief Velocity inherited from the source entity.
         *
         * @details Prevents projectiles from spawning "behind" a fast-moving shooter.
         */
        const helios::math::vec3f inheritedVelocity;

        /**
         * @brief Fire intensity from trigger pressure (0.0 to 1.0).
         *
         * @details Affects spawn rate accumulation in the ProjectilePoolManager.
         * Higher values result in faster fire rates.
         */
        const float intensity;
    };
    
    
}