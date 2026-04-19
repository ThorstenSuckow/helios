/**
 * @file EmitterInitializer.ixx
 * @brief Initializer that configures spawned entities based on emitter state.
 */
module;

#include <cassert>

export module helios.gameplay.spawn.behavior.initializers.EmitterInitializer;

import helios.gameplay.spawn.behavior.SpawnInitializer;
import helios.gameplay.spawn.types.SpawnPlanCursor;
import helios.gameplay.spawn.types.SpawnContext;
import helios.runtime.world.GameObject;
import helios.physics.motion.components.Move2DComponent;
import helios.physics.motion.components.DirectionComponent;
import helios.physics.motion.components.SteeringComponent;


using namespace helios::gameplay::spawn::types;
export namespace helios::gameplay::spawn::behavior::initializers {

    /**
     * @brief Initializer that configures spawned entities based on emitter state.
     *
     * @details EmitterInitializer uses the velocity from the EmitterContext to
     * set the direction and movement intent of spawned entities. This is commonly
     * used for projectiles that should inherit direction from the firing entity.
     *
     * Components affected:
     * - **Move2DComponent:** Sets move intent in emitter's direction
     * - **DirectionComponent:** Sets direction to normalized emitter velocity
     * - **SteeringComponent:** Sets steering intent in emitter's direction
     *
     * @pre The SpawnContext must contain a valid EmitterContext.
     *
     * @see SpawnInitializer
     * @see EmitterContext
     */
    template<typename THandle>
    class EmitterInitializer final : public SpawnInitializer<THandle> {

    public:

        /**
         * @brief Initializes entity with emitter's velocity direction.
         *
         * @param gameObject The entity to initialize.
         * @param cursor The current position within the spawn batch (unused).
         * @param spawnContext Context containing the emitter information.
         *
         * @pre spawnContext.emitterContext must have a value.
         */
        void initialize(
            helios::runtime::world::GameObject gameObject,
            const SpawnPlanCursor& cursor,
            const SpawnContext<THandle>& spawnContext
        ) override {

            const auto& emitterContext = spawnContext.emitterContext;
            assert(emitterContext.has_value() && "Unexpected missing value for emitterContext");

            const auto velocity = emitterContext.value().velocity;
            auto direction = velocity.normalize();

            if (auto* m2c = gameObject.template get<helios::physics::motion::components::Move2DComponent<THandle>>()) {
                m2c->setMoveIntent(direction, 1.0f);
            }

            if (auto* dc = gameObject.template get<helios::physics::motion::components::DirectionComponent<THandle>>()) {
                dc->setDirection(direction);
            }

            if (auto* hc = gameObject.template get<helios::physics::motion::components::SteeringComponent<THandle>>()) {
                hc->setSteeringIntent(direction, 1.0f);
            }
        }

    };

}
