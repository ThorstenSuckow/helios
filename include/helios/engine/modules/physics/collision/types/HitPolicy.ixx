/**
 * @file HitPolicy.ixx
 * @brief Defines collision hit policies controlling how many collisions are reported per frame.
 */
module;


export module helios.engine.modules.physics.collision.types.HitPolicy;


export namespace helios::engine::modules::physics::collision::types {

    /**
     * @brief Specifies how many collision events an entity can receive per frame.
     *
     * @details HitPolicy controls the collision detection behavior when an entity
     * overlaps multiple other entities in the same frame. This affects both
     * performance and gameplay logic.
     *
     * ## Policies
     *
     * | Policy | Behavior |
     * |--------|----------|
     * | `OneHit` | Reports only the first collision detected (default) |
     * | `All` | Reports all collisions with overlapping entities |
     *
     * ## Use Cases
     *
     * - **OneHit:** Projectiles that destroy on first contact, pickups collected once
     * - **All:** Area-of-effect damage, entities affecting multiple targets simultaneously
     *
     * ## Usage
     *
     * ```cpp
     * // Projectile: stops after first hit
     * prototype.withCollision([](CollisionBuilder& b) {
     *     b.collision()
     *      .layer(Layers::PROJECTILE)
     *      .hitPolicy(HitPolicy::OneHit);
     * });
     *
     * // Explosion: damages all entities in range
     * prototype.withCollision([](CollisionBuilder& b) {
     *     b.collision()
     *      .layer(Layers::EXPLOSION)
     *      .hitPolicy(HitPolicy::All);
     * });
     * ```
     *
     * @see CollisionComponent::setHitPolicy
     * @see GridCollisionDetectionSystem
     */
    enum class HitPolicy {

        /**
         * @brief Reports only the first collision per frame.
         *
         * @details Once a collision is detected, the entity skips further
         * collision checks for the current frame. This is the default policy
         * and provides optimal performance for typical projectile behavior.
         */
        OneHit,

        /**
         * @brief Reports all collisions with overlapping entities.
         *
         * @details The entity continues collision checks even after detecting
         * a collision, generating events for every overlapping entity. Use for
         * area-of-effect or multi-target interactions.
         */
        All

    };


}