/**
 * @file DirectionInitializer.ixx
 * @brief Spawn initializer that sets initial movement direction for spawned entities.
 */
module;

export module helios.engine.runtime.spawn.behavior.initializers.DirectionInitializer;

import helios.engine.runtime.spawn.behavior.SpawnInitializer;
import helios.engine.runtime.spawn.SpawnPlanCursor;
import helios.engine.runtime.spawn.SpawnContext;
import helios.engine.ecs.GameObject;
import helios.engine.modules.physics.motion.components.Move2DComponent;
import helios.engine.modules.physics.motion.components.DirectionComponent;
import helios.math;
import helios.util.Random;

export namespace helios::engine::runtime::spawn::behavior::initializers {

    /**
     * @brief Specifies the direction initialization strategy.
     */
    enum class Direction {

        /**
         * @brief Assigns a random normalized 2D direction.
         */
        Random,

        /**
         * @brief Assigns a horizontal direction (positive X-axis).
         */
        Horizontal

    };

    /**
     * @brief Initializer that sets movement direction for spawned entities.
     *
     * @details DirectionInitializer configures the DirectionComponent and
     * Move2DComponent of spawned entities based on the specified Direction
     * strategy. This determines the initial movement vector of the entity.
     *
     * ## Direction Strategies
     *
     * - **Random:** Generates a random normalized 2D direction using a seeded RNG
     * - **Horizontal:** Sets direction to positive X-axis (1, 0, 0)
     *
     * ## Required Components
     *
     * Spawned entities must have:
     * - `DirectionComponent` — receives the calculated direction
     * - `Move2DComponent` — receives the move command with direction
     *
     * Example:
     * ```cpp
     * auto initializer = std::make_unique<DirectionInitializer>(Direction::Random);
     * spawnProfile.spawnInitializer = std::move(initializer);
     * ```
     *
     * @see SpawnInitializer
     * @see DirectionComponent
     * @see Move2DComponent
     */
    class DirectionInitializer final : public SpawnInitializer {

        /**
         * @brief The direction strategy to apply during initialization.
         */
        const Direction direction_ = Direction::Random;

        /**
         * @brief Applies a random direction to the entity.
         *
         * @details Generates a random 2D vector, normalizes it, and sets it
         * as the entity's direction. Also triggers a move command with
         * full throttle (1.0).
         *
         * @param gameObject The spawned entity to initialize.
         * @param cursor The spawn plan cursor (unused).
         * @param spawnContext The spawn context (unused).
         */
        void random(
            const helios::engine::ecs::GameObject& gameObject,
            const SpawnPlanCursor& cursor,
            const SpawnContext& spawnContext
        ) const noexcept {
            static auto rGen = helios::util::Random(12345);

            auto* mc = gameObject.get<helios::engine::modules::physics::motion::components::Move2DComponent>();
            auto* dc = gameObject.get<helios::engine::modules::physics::motion::components::DirectionComponent>();

            auto dir = helios::math::vec2f{
                rGen.randomFloat(-1.0f, 1.0f),
                rGen.randomFloat(-1.0f, 1.0f)
            };

            dc->setDirection(dir.normalize().toVec3());
            mc->move(dc->direction(), 1.0f);
        }

        /**
         * @brief Applies a horizontal direction to the entity.
         *
         * @details Sets the entity's direction to the positive X-axis (1, 0, 0)
         * and triggers a move command with full throttle (1.0).
         *
         * @param gameObject The spawned entity to initialize.
         * @param cursor The spawn plan cursor (unused).
         * @param spawnContext The spawn context (unused).
         */
        void horizontal (
            const helios::engine::ecs::GameObject& gameObject,
            const SpawnPlanCursor& cursor,
            const SpawnContext& spawnContext
        ) const noexcept {

            static auto rGen = helios::util::Random(12345);

            auto* mc = gameObject.get<helios::engine::modules::physics::motion::components::Move2DComponent>();
            auto* dc = gameObject.get<helios::engine::modules::physics::motion::components::DirectionComponent>();

            auto dir = helios::math::vec2f{
                1.0f, 0.0f
            };

            dc->setDirection(dir.normalize().toVec3());
            mc->move(dc->direction(), 1.0f);


        }
    public:

        /**
         * @brief Constructs a DirectionInitializer with the specified strategy.
         *
         * @param direction The direction strategy to use during initialization.
         */
        explicit DirectionInitializer(const Direction direction) : direction_(direction) {}


        /**
         * @brief Initializes the spawned entity's direction and movement.
         *
         * @details Delegates to the appropriate strategy method based on
         * the configured Direction enum value.
         *
         * @param gameObject The spawned entity to initialize.
         * @param cursor The spawn plan cursor providing batch context.
         * @param spawnContext The spawn context with emitter information.
         */
        void initialize(
            const helios::engine::ecs::GameObject& gameObject,
            const SpawnPlanCursor& cursor,
            const SpawnContext& spawnContext
        ) override {

            switch (direction_) {
                case Direction::Random:
                    random(gameObject, cursor, spawnContext);
                    return;
                case Direction::Horizontal:
                    horizontal(gameObject, cursor, spawnContext);
                    return;
            }

        }

    };

}