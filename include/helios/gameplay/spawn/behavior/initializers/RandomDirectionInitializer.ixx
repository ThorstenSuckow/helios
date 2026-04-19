/**
 * @file RandomDirectionInitializer.ixx
 * @brief Initializer that configures spawned entities with random direction.
 */
module;

#include <type_traits>

export module helios.gameplay.spawn.behavior.initializers.RandomDirectionInitializer;

import helios.gameplay.spawn.behavior.SpawnInitializer;
import helios.gameplay.spawn.types.SpawnPlanCursor;
import helios.gameplay.spawn.types.SpawnContext;
import helios.physics.motion.components.Move2DComponent;
import helios.physics.motion.components.DirectionComponent;
import helios.math;
import helios.util.Random;

using namespace helios::gameplay::spawn::types;
export namespace helios::gameplay::spawn::behavior::initializers {

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
    class RandomDirectionInitializer {

        helios::util::Random rGen_{12345};

    public:

        /**
         * @brief Initializes entity with a random direction.
         *
         * @param entity The entity to initialize.
         * @param cursor The current position within the spawn batch (unused).
         * @param spawnContext Context data (unused).
         */
        template<typename TEntity>
        requires std::is_same_v<typename TEntity::Handle_type, THandle>
        void initialize(
            TEntity entity,
            const SpawnPlanCursor& cursor,
            const SpawnContext<THandle>& spawnContext
        ) {


            auto* mc = entity.template get<helios::physics::motion::components::Move2DComponent<THandle>>();
            auto* dc = entity.template get<helios::physics::motion::components::DirectionComponent<THandle>>();

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
        void onReset() noexcept  {
            rGen_.reset();
        }

    };

}