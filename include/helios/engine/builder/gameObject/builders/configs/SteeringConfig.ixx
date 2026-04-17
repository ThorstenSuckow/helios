/**
 * @file SteeringConfig.ixx
 * @brief Configuration for steering/rotation components.
 */
module;

export module helios.engine.builder.gameObject.builders.configs.SteeringConfig;

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
    template<typename Entity>
    class SteeringConfig {

        using Handle_type = typename Entity::Handle_type;

        Entity gameObject_;

    public:

        /**
         * @brief Constructs a SteeringConfig and adds required components.
         *
         * @param gameObject Target GameObject to configure.
         */
        explicit SteeringConfig(Entity gameObject) : gameObject_(gameObject) {
            gameObject_.template add<helios::engine::modules::physics::motion::components::SteeringComponent<Handle_type>>();

            gameObject_.template getOrAdd<helios::engine::modules::spatial::transform::components::ComposeTransformComponent<Handle_type>>();
            gameObject_.template getOrAdd<helios::engine::modules::spatial::transform::components::RotationStateComponent<Handle_type>>();
            gameObject_.template getOrAdd<helios::engine::modules::physics::motion::components::DirectionComponent<Handle_type>>();
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
            gameObject_.get<helios::engine::modules::physics::motion::components::SteeringComponent<Handle_type>>()
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
            gameObject_.get<helios::engine::modules::physics::motion::components::SteeringComponent<Handle_type>>()
                      ->setDirectionFromSteering(directionFromSteering);

            return *this;
        }

    };

}

