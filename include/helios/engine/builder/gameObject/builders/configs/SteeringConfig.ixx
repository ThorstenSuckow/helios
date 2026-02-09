/**
 * @file SteeringConfig.ixx
 * @brief Configuration for steering/rotation components.
 */
module;

export module helios.engine.builder.gameObject.builders.configs.SteeringConfig;

import helios.engine.ecs.GameObject;
import helios.engine.modules.physics;
import helios.engine.modules.spatial;

export namespace helios::engine::builder::gameObject::builders::configs {

    /**
     * @brief Fluent configuration for steering and rotation behavior.
     *
     * Automatically adds required components:
     * - SteeringComponent
     * - ComposeTransformComponent
     * - RotationStateComponent
     * - DirectionComponent
     */
    class SteeringConfig {

        /**
         * @brief Non-owning pointer to the target GameObject.
         */
        helios::engine::ecs::GameObject gameObject_;

    public:

        /**
         * @brief Constructs a SteeringConfig and adds required components.
         *
         * @param gameObject Target GameObject to configure.
         */
        explicit SteeringConfig(helios::engine::ecs::GameObject gameObject) : gameObject_(gameObject) {
            gameObject_.add<helios::engine::modules::physics::motion::components::SteeringComponent>();

            gameObject_.getOrAdd<helios::engine::modules::spatial::transform::components::ComposeTransformComponent>();
            gameObject_.getOrAdd<helios::engine::modules::spatial::transform::components::RotationStateComponent>();
            gameObject_.getOrAdd<helios::engine::modules::physics::motion::components::DirectionComponent>();
        }

        /**
         * @brief Enables or disables instant rotation.
         *
         * When enabled, the entity rotates immediately to face target.
         *
         * @param useInstantRotation True for instant rotation.
         *
         * @return Reference to this config for chaining.
         */
        SteeringConfig& instantSteering(const bool useInstantRotation) {
            gameObject_.get<helios::engine::modules::physics::motion::components::SteeringComponent>()
                      ->setUseInstantRotation(useInstantRotation);

            return *this;
        }

        /**
         * @brief Sets whether steering input updates movement direction.
         *
         * @param directionFromSteering True to link direction to steering.
         *
         * @return Reference to this config for chaining.
         */
        SteeringConfig& steeringSetsDirection(const bool directionFromSteering) {
            gameObject_.get<helios::engine::modules::physics::motion::components::SteeringComponent>()
                      ->setDirectionFromSteering(directionFromSteering);

            return *this;
        }

    };

}

