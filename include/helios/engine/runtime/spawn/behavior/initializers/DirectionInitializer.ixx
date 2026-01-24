/**
 * @file DirectionInitializer.ixx
 * @brief Spawn initializer that sets initial movement direction for spawned entities.
 */
module;

#include <cassert>
#include <cmath>

export module helios.engine.runtime.spawn.behavior.initializers.DirectionInitializer;

import helios.engine.runtime.spawn.behavior.SpawnInitializer;
import helios.engine.runtime.spawn.SpawnPlanCursor;
import helios.engine.runtime.spawn.SpawnContext;
import helios.engine.ecs.GameObject;
import helios.engine.modules.physics.motion.components.Move2DComponent;
import helios.engine.modules.physics.motion.components.DirectionComponent;

import helios.engine.modules.spatial.transform.components.RotationStateComponent;
import helios.engine.modules.physics.motion.components.SteeringComponent;

import helios.math;
import helios.util.Random;


export namespace helios::engine::runtime::spawn::behavior::initializers {

    /**
     * @brief Specifies the direction initialization strategy.
     */
    enum class Direction {

        /**
         * @brief Generates a random normalized direction vector.
         */
        Random,

        /**
         * @brief Sets direction to positive X-axis.
         */
        Right,

        /**
         * @brief Sets direction to negative X-axis.
         */
        Left,

        /**
         * @brief Sets direction to a custom axis vector.
         */
        Axis

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
     * - **Random:** Generates a random normalized 2D direction using a seeded RNG.
     * - **Right:** Sets direction to positive X-axis (1, 0, 0).
     * - **Left:** Sets direction to negative X-axis (-1, 0, 0).
     * - **Axis:** Sets direction to a custom provided axis.
     *
     * ## Required Components
     *
     * Spawned entities must have:
     * - `DirectionComponent` — receives the calculated direction.
     * - `Move2DComponent` — receives the move command with direction.
     * - `SteeringComponent` (Optional) — receives the rotation looking at direction.
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
         * @brief Stores the custom direction axis when using Direction::Axis.
         */
        helios::math::vec3f directionAxis_{};

        /**
         * @brief Applies a random direction to the entity.
         *
         * @details Generates a random 2D vector, normalizes it, and sets it
         * as the entity's direction. Also triggers a move command with
         * full throttle (1.0).
         *
         * If a SteeringComponent is present, the entity is rotated to face the
         * movement direction.
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
            auto* sc = gameObject.get<helios::engine::modules::physics::motion::components::SteeringComponent>();
            if (sc) {
                sc->setTargetRotationAngle(helios::math::degrees(std::atan2(dir[1], dir[0])));
                sc->setCurrentRotationAngle(helios::math::degrees(std::atan2(dir[1], dir[0])));
            }

            dc->setDirection(dir.normalize().toVec3());
            mc->move(dc->direction(), 1.0f);
        }

        /**
         * @brief Applies a specific direction axis to the entity.
         *
         * @details Sets the entity's direction to the provided axis vector
         * and triggers a move command with full throttle (1.0).
         *
         * If a SteeringComponent is present, the entity is rotated to face the
         * movement direction.
         *
         * @param gameObject The spawned entity to initialize.
         * @param cursor The spawn plan cursor (unused).
         * @param spawnContext The spawn context (unused).
         * @param directionAxis The normalized direction vector to apply.
         */
        void alignToAxis (
            const helios::engine::ecs::GameObject& gameObject,
            const SpawnPlanCursor& cursor,
            const SpawnContext& spawnContext,
            const helios::math::vec3f directionAxis
        ) const noexcept {

            static auto rGen = helios::util::Random(12345);

            auto* mc = gameObject.get<helios::engine::modules::physics::motion::components::Move2DComponent>();
            auto* sc = gameObject.get<helios::engine::modules::physics::motion::components::SteeringComponent>();
            auto* dc = gameObject.get<helios::engine::modules::physics::motion::components::DirectionComponent>();

            assert(directionAxis.isNormalized() && "horizontal initializer requires valid direction vector");
            dc->setDirection(directionAxis);

            sc->setTargetRotationAngle(helios::math::degrees(std::atan2(directionAxis[1], directionAxis[0])));
            sc->setCurrentRotationAngle(helios::math::degrees(std::atan2(directionAxis[1], directionAxis[0])));

            mc->move(directionAxis, 1.0f);


        }
    public:

        /**
         * @brief Constructs a DirectionInitializer with the specified strategy.
         *
         * @param direction The direction strategy to use during initialization.
         */
        explicit DirectionInitializer(const Direction direction) : direction_(direction) {}

        /**
         * @brief Constructs a DirectionInitializer with a custom axis.
         *
         * @details Sets the strategy to `Direction::Axis` and stores the provided
         * axis vector.
         *
         * @param directionAxis The custom direction vector to use.
         */
        explicit DirectionInitializer(const helios::math::vec3f directionAxis) : direction_(Direction::Axis), directionAxis_(directionAxis) {}


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
                case Direction::Right:
                    alignToAxis(gameObject, cursor, spawnContext, helios::math::X_AXISf);
                    return;
                case Direction::Left:
                    alignToAxis(gameObject, cursor, spawnContext, helios::math::X_AXISf * -1.0f);
                    return;
                case Direction::Axis:
                    alignToAxis(gameObject, cursor, spawnContext, directionAxis_);
                    return;
            }



        }

    };

}


