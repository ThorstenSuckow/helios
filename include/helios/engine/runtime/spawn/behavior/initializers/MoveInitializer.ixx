/**
 * @file MoveInitializer.ixx
 * @brief Spawn initializer that sets initial movement direction for spawned entities.
 */
module;

#include <cassert>
#include <cmath>

export module helios.engine.runtime.spawn.behavior.initializers.MoveInitializer;

import helios.engine.runtime.spawn.behavior.SpawnInitializer;
import helios.engine.runtime.spawn.SpawnPlanCursor;
import helios.engine.runtime.spawn.SpawnContext;
import helios.engine.ecs.GameObject;
import helios.engine.modules.physics.motion.components.Move2DComponent;
import helios.engine.modules.physics.motion.components.DirectionComponent;

import helios.engine.modules.spatial.transform.components.TranslationStateComponent;

import helios.engine.modules.spatial.transform.components.RotationStateComponent;
import helios.engine.modules.physics.motion.components.SteeringComponent;

import helios.math;
import helios.util.Random;


export namespace helios::engine::runtime::spawn::behavior::initializers {

    /**
     * @brief Specifies the direction initialization strategy.
     */
    enum class DirectionType {

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
        Axis,

        /**
         * @brief Sets direction toward a specific target point.
         *
         * @details The direction is calculated as the normalized vector from
         * the entity's current position to the specified target point.
         */
        Point
    };

    /**
     * @brief Initializer that sets movement direction for spawned entities.
     *
     * @details MoveInitializer configures the DirectionComponent and
     * Move2DComponent of spawned entities based on the specified DirectionType
     * strategy. This determines the initial movement vector of the entity.
     *
     * ## Direction Strategies
     *
     * - **Random:** Generates a random normalized 2D direction using a seeded RNG.
     * - **Right:** Sets direction to positive X-axis (1, 0, 0).
     * - **Left:** Sets direction to negative X-axis (-1, 0, 0).
     * - **Axis:** Sets direction to a custom provided axis vector.
     * - **Point:** Calculates direction toward a specific target point.
     *
     * ## Required Components
     *
     * Spawned entities must have:
     * - `DirectionComponent` — receives the calculated direction.
     * - `Move2DComponent` — receives the move command with direction.
     * - `SteeringComponent` (Optional) — receives the rotation looking at direction.
     * - `TranslationStateComponent` (Required for `Point` strategy) — provides current position.
     *
     * ## Usage Examples
     *
     * Random direction:
     * ```cpp
     * auto initializer = std::make_unique<MoveInitializer>(DirectionType::Random);
     * spawnProfile.spawnInitializer = std::move(initializer);
     * ```
     *
     * Custom axis direction:
     * ```cpp
     * auto axis = helios::math::vec3f{0.707f, 0.707f, 0.0f}; // 45 degrees
     * auto initializer = std::make_unique<MoveInitializer>(axis);
     * ```
     *
     * Direction toward a target point:
     * ```cpp
     * auto target = helios::math::vec3f{100.0f, 50.0f, 0.0f};
     * auto initializer = std::make_unique<MoveInitializer>(target, DirectionType::Point);
     * ```
     *
     * @see SpawnInitializer
     * @see DirectionComponent
     * @see Move2DComponent
     * @see TranslationStateComponent
     */
    class MoveInitializer final : public SpawnInitializer {

        helios::util::Random rGen_{12345};

        /**
         * @brief The direction strategy to apply during initialization.
         */
        const DirectionType directionType_ = DirectionType::Random;

        /**
         * @brief Stores the custom direction target (axis/point) when using DirectionType::Axis/::Point.
         */
        helios::math::vec3f direction_{};

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
            helios::engine::ecs::GameObject gameObject,
            const SpawnPlanCursor& cursor,
            const SpawnContext& spawnContext
        ) noexcept {

            auto* mc = gameObject.get<helios::engine::modules::physics::motion::components::Move2DComponent>();
            auto* dc = gameObject.get<helios::engine::modules::physics::motion::components::DirectionComponent>();

            auto dir = helios::math::vec2f{
                rGen_.randomFloat(-1.0f, 1.0f),
                rGen_.randomFloat(-1.0f, 1.0f)
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
         * @brief Aligns the entity's movement direction to a target axis or point.
         *
         * @details Depending on the directionType parameter:
         * - **Axis:** Sets the entity's direction to the provided normalized vector.
         * - **Point:** Calculates the direction from the entity's current position
         *   to the target point and normalizes it.
         *
         * In both cases, triggers a move command with full throttle (1.0) and
         * updates the SteeringComponent to rotate the entity toward the movement
         * direction.
         *
         * @param gameObject The spawned entity to initialize.
         * @param cursor The spawn plan cursor (unused).
         * @param spawnContext The spawn context (unused).
         * @param target The target axis vector or point coordinates.
         * @param directionType The direction strategy (`Axis` or `Point`).
         */
        void alignTo (
            helios::engine::ecs::GameObject gameObject,
            const SpawnPlanCursor& cursor,
            const SpawnContext& spawnContext,
            const helios::math::vec3f target,
            const DirectionType directionType = DirectionType::Axis
            
        ) noexcept {

            auto* mc = gameObject.get<helios::engine::modules::physics::motion::components::Move2DComponent>();
            auto* sc = gameObject.get<helios::engine::modules::physics::motion::components::SteeringComponent>();
            auto* dc = gameObject.get<helios::engine::modules::physics::motion::components::DirectionComponent>();

            auto direction = target;
            if (directionType == DirectionType::Axis) {
                assert(direction.isNormalized() && "axis initializer requires valid direction vector");
            } else if (directionType == DirectionType::Point) {
                auto* tsc = gameObject.get<helios::engine::modules::spatial::transform::components::TranslationStateComponent>();

                direction = (target - (tsc->translation() * -1.0f)).normalize();
                assert(direction.isNormalized() && "point initializer requires valid direction vector");
            }
            
            dc->setDirection(direction);

            sc->setTargetRotationAngle(helios::math::degrees(std::atan2(direction[1], direction[0])));
            sc->setCurrentRotationAngle(helios::math::degrees(std::atan2(direction[1], direction[0])));

            mc->move(direction, 1.0f);


        }
    public:

        /**
         * @brief Constructs a MoveInitializer with the specified strategy.
         *
         * @param direction The direction strategy to use during initialization.
         */
        explicit MoveInitializer(const DirectionType direction) : directionType_(direction) {}

        /**
         * @brief Constructs a MoveInitializer with a custom axis.
         *
         * @details Sets the strategy to `DirectionType::Axis` and stores the provided
         * axis vector.
         *
         * @param directionAxis The custom direction vector to use.
         */
        explicit MoveInitializer(const helios::math::vec3f directionAxis) : directionType_(DirectionType::Axis), direction_(directionAxis) {}

        /**
         * @brief Constructs a MoveInitializer with a target point or axis and explicit strategy.
         *
         * @details Allows specifying both a target vector and the direction strategy.
         * Use `DirectionType::Point` to calculate direction toward the target point,
         * or `DirectionType::Axis` to use the vector as a direct direction.
         *
         * @param target The target point or axis vector.
         * @param direction The direction strategy to use.
         */
        explicit MoveInitializer(const helios::math::vec3f target, const DirectionType direction) : directionType_(direction), direction_(target) {}


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
            helios::engine::ecs::GameObject gameObject,
            const SpawnPlanCursor& cursor,
            const SpawnContext& spawnContext
        ) override {

            switch (directionType_) {
                case DirectionType::Random:
                    random(gameObject, cursor, spawnContext);
                    return;
                case DirectionType::Right:
                    alignTo(gameObject, cursor, spawnContext, helios::math::X_AXISf);
                    return;
                case DirectionType::Left:
                    alignTo(gameObject, cursor, spawnContext, helios::math::X_AXISf * -1.0f);
                    return;
                case DirectionType::Axis:
                    alignTo(gameObject, cursor, spawnContext, direction_);
                    return;
                    
                case DirectionType::Point:
                    alignTo(gameObject, cursor, spawnContext, direction_, directionType_);
                    return;
            }
        }

        void onReset() noexcept override  {
            rGen_.reset();
        }
    };

}


