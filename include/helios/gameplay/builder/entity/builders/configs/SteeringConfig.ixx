/**
 * @file SteeringConfig.ixx
 * @brief Configuration for steering/rotation components.
 */
module;

export module helios.gameplay.builder.entity.builders.configs.SteeringConfig;

import helios.physics;
import helios.spatial;

export namespace helios::gameplay::builder::entity::builders::configs {

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

        Entity entity_;

    public:

        /**
         * @brief Constructs a SteeringConfig and adds required components.
         *
         * @param entity Target Entity to configure.
         */
        explicit SteeringConfig(Entity entity) : entity_(entity) {
            entity_.template add<helios::physics::motion::components::SteeringComponent<Handle_type>>();

            entity_.template getOrAdd<helios::spatial::transform::components::ComposeTransformComponent<Handle_type>>();
            entity_.template getOrAdd<helios::spatial::transform::components::RotationStateComponent<Handle_type>>();
            entity_.template getOrAdd<helios::physics::motion::components::DirectionComponent<Handle_type>>();
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
            entity_.get<helios::physics::motion::components::SteeringComponent<Handle_type>>()
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
            entity_.get<helios::physics::motion::components::SteeringComponent<Handle_type>>()
                      ->setDirectionFromSteering(directionFromSteering);

            return *this;
        }

    };

}

