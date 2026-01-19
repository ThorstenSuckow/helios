/**
 * @file EmitterInitializer.ixx
 * @brief Initializer that configures spawned entities based on emitter state.
 */
module;

#include <cassert>

export module helios.engine.runtime.spawn.behavior.initializers.EmitterInitializer;

import helios.engine.runtime.spawn.behavior.SpawnInitializer;
import helios.engine.runtime.spawn.SpawnPlanCursor;
import helios.engine.runtime.spawn.SpawnContext;
import helios.engine.ecs.GameObject;
import helios.engine.modules.physics.motion.components.Move2DComponent;
import helios.engine.modules.physics.motion.components.DirectionComponent;
import helios.engine.modules.physics.motion.components.SteeringComponent;

export namespace helios::engine::runtime::spawn::behavior::initializers {

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
    class EmitterInitializer final : public SpawnInitializer {

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
            const helios::engine::ecs::GameObject& gameObject,
            const SpawnPlanCursor& cursor,
            const SpawnContext& spawnContext
        ) override {

            const auto& emitterContext = spawnContext.emitterContext;
            assert(emitterContext.has_value() && "Unexpected missing value for emitterContext");

            const auto velocity = emitterContext.value().velocity;
            auto direction = velocity.normalize();

            if (auto* m2c = gameObject.get<helios::engine::modules::physics::motion::components::Move2DComponent>()) {
                m2c->setMoveIntent(direction, 1.0f);
            }

            if (auto* dc = gameObject.get<helios::engine::modules::physics::motion::components::DirectionComponent>()) {
                dc->setDirection(direction);
            }

            if (auto* hc = gameObject.get<helios::engine::modules::physics::motion::components::SteeringComponent>()) {
                hc->setSteeringIntent(direction, 1.0f);
            }
        }

    };

}
