/**
 * @file RandomDirectionInitializer.ixx
 * @brief Initializer that configures spawned entities with random direction.
 */
module;

export module helios.runtime.spawn.behavior.initializers.RandomDirectionInitializer;

import helios.runtime.spawn.behavior.SpawnInitializer;
import helios.runtime.spawn.types.SpawnPlanCursor;
import helios.runtime.spawn.types.SpawnContext;
import helios.runtime.world.GameObject;
import helios.engine.modules.physics.motion.components.Move2DComponent;
import helios.engine.modules.physics.motion.components.DirectionComponent;
import helios.math;
import helios.util.Random;

using namespace helios::runtime::spawn::types;
export namespace helios::runtime::spawn::behavior::initializers {

    /**
     * @brief Initializer that configures spawned entities with random direction.
     *
     * @details RandomDirectionInitializer generates a random 2D direction and
     * applies it to the entity's DirectionComponent and Move2DComponent. This is
     * useful for spawning enemies that should move in unpredictable directions.
     *
     * Components affected:
     * - **DirectionComponent:** Sets random normalized direction
     * - **Move2DComponent:** Sets move intent in the random direction
     *
     * @note Uses a static Random generator with a fixed seed for reproducibility.
     *
     * @see SpawnInitializer
     */
    template<typename THandle>
    class RandomDirectionInitializer final : public SpawnInitializer<THandle> {

        helios::util::Random rGen_{12345};

    public:

        /**
         * @brief Initializes entity with a random direction.
         *
         * @param gameObject The entity to initialize.
         * @param cursor The current position within the spawn batch (unused).
         * @param spawnContext Context data (unused).
         */
        void initialize(
            helios::runtime::world::GameObject gameObject,
            const SpawnPlanCursor& cursor,
            const SpawnContext<THandle>& spawnContext
        ) override {


            auto* mc = gameObject.get<helios::engine::modules::physics::motion::components::Move2DComponent>();
            auto* dc = gameObject.get<helios::engine::modules::physics::motion::components::DirectionComponent>();

            auto dir = helios::math::vec2f{
                rGen_.randomFloat(-1.0f, 1.0f),
                rGen_.randomFloat(-1.0f, 1.0f)
            };

            dc->setDirection(dir.normalize().toVec3());
            mc->move(dc->direction(), 1.0f);
        }

        /**
         * @brief Resets the random number generator to its initial seed.
         */
        void onReset() noexcept override  {
            rGen_.reset();
        }

    };

}