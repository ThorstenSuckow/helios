/**
 * @file SpawnInitializer.ixx
 * @brief Abstract interface for initializing spawned entity state.
 */
module;

export module helios.engine.runtime.spawn.behavior.SpawnInitializer;

import helios.engine.runtime.spawn.SpawnPlanCursor;
import helios.engine.runtime.spawn.SpawnContext;
import helios.engine.ecs.GameObject;

export namespace helios::engine::runtime::spawn::behavior {

    /**
     * @brief Abstract interface for initializing spawned entity state.
     *
     * @details SpawnInitializer defines the strategy for configuring a spawned
     * entity's components after it has been placed. Implementations can set
     * velocity, direction, stats, or any other entity-specific state.
     *
     * Common implementations:
     *
     * - **ProjectileInitializer:** Sets velocity and direction
     * - **EnemyInitializer:** Configures health, AI state, behavior
     * - **PickupInitializer:** Sets pickup type and value
     *
     * Example implementation:
     * ```cpp
     * class ProjectileInitializer : public SpawnInitializer {
     *     float speed_;
     * public:
     *     explicit ProjectileInitializer(float speed) : speed_(speed) {}
     *
     *     void initialize(const GameObject go, const SpawnPlanCursor& cursor,
     *                     const SpawnContext& ctx) override {
     *         if (auto* dir = go.get<DirectionComponent>()) {
     *             dir->setDirection(ctx.emitterContext->velocity.normalized());
     *         }
     *         if (auto* move = go.get<Move2DComponent>()) {
     *             move->setSpeed(speed_);
     *         }
     *     }
     * };
     * ```
     *
     * @see SpawnProfile
     * @see SpawnPlacer
     * @see SpawnContext
     */
    class SpawnInitializer {

    public:

        virtual ~SpawnInitializer() = default;

        /**
         * @brief Initializes a spawned entity's state.
         *
         * @param gameObject The entity to initialize.
         * @param cursor The current position within the spawn batch.
         * @param spawnContext Context data including optional emitter info.
         */
        virtual void initialize(
            helios::engine::ecs::GameObject gameObject,
            const SpawnPlanCursor& cursor,
            const SpawnContext& spawnContext
        ) = 0;


    };

}