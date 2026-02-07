/**
 * @file EmitterSpawnPlacer.ixx
 * @brief Placer that spawns entities at the emitter's position.
 */
module;

#include <cassert>

export module helios.engine.runtime.spawn.behavior.placements.EmitterSpawnPlacer;

import helios.engine.runtime.spawn.SpawnPlanCursor;
import helios.engine.runtime.spawn.SpawnContext;
import helios.engine.runtime.spawn.behavior.SpawnPlacer;
import helios.math;
import helios.engine.ecs.EntityHandle;

export namespace helios::engine::runtime::spawn::behavior::placements {

    /**
     * @brief Placer that spawns entities at the emitter's position.
     *
     * @details EmitterSpawnPlacer uses the position stored in the EmitterContext
     * to place spawned entities. This is commonly used for projectiles that should
     * originate from the firing entity.
     *
     * @pre The SpawnContext must contain a valid EmitterContext.
     *
     * @see SpawnPlacer
     * @see EmitterContext
     */
    class EmitterSpawnPlacer final : public SpawnPlacer {

    public:

        /**
         * @brief Returns the emitter's position.
         *
         * @param entityHandle The handle of the entity being spawned (unused).
         * @param gameObjectBounds The bounding box of the entity to spawn (unused).
         * @param environmentBounds The level bounds (unused).
         * @param cursor The current position within the spawn batch (unused).
         * @param spawnContext Context containing the emitter information.
         *
         * @return The emitter's world position.
         *
         * @pre spawnContext.emitterContext must have a value.
         */
        helios::math::vec3f getPosition(
            const helios::engine::ecs::EntityHandle& entityHandle,
            const helios::math::aabbf& gameObjectBounds,
            const helios::math::aabbf& environmentBounds,
            const SpawnPlanCursor& cursor,
            const SpawnContext& spawnContext
        ) override {

            const auto emitterCtx = spawnContext.emitterContext;
            assert(emitterCtx.has_value() && "Unexpected nullopt for emitter");

            return emitterCtx.value().position;
        }


    };

}