/**
 * @file Move2DConfig.ixx
 * @brief Configuration for 2D movement components.
 */
module;

export module helios.gameplay.builder.entity.builders.configs.Move2DConfig;

import helios.physics;
import helios.spatial;

export namespace helios::gameplay::builder::entity::builders::configs {

    /**
     * @brief Fluent configuration for 2D movement behavior.
     *
     * Automatically adds required components:
     * - Move2DComponent
     * - ComposeTransformComponent
     * - DirectionComponent
     * - TranslationStateComponent
     */
    template<typename Entity>
    class Move2DConfig {

        using Handle_type = typename Entity::Handle_type;

        Entity entity_;

    public:

        /**
         * @brief Constructs a Move2DConfig and adds required components.
         *
         * @param entity Target Entity to configure.
         */
        explicit Move2DConfig(Entity entity) : entity_(entity) {

            entity_.template add<helios::physics::motion::components::Move2DComponent<Handle_type>>();

            entity_.template getOrAdd<helios::spatial::transform::components::ComposeTransformComponent<Handle_type>>();
            entity_.template getOrAdd<helios::physics::motion::components::DirectionComponent<Handle_type>>();
            entity_.template getOrAdd<helios::spatial::transform::components::TranslationStateComponent<Handle_type>>();

        }

        /**
         * @brief Sets the maximum movement speed.
         *
         * @param movementSpeed Speed in units per second.
         *
         * @return Reference to this config for chaining.
         */
        Move2DConfig& speed(const float movementSpeed) {
            entity_.template get<helios::physics::motion::components::Move2DComponent<Handle_type>>()
                      ->setMovementSpeed(movementSpeed);

            return *this;
        }

        /**
         * @brief Sets the movement acceleration rate.
         *
         * @param movementAcceleration Acceleration in units per second squared.
         *
         * @return Reference to this config for chaining.
         */
        Move2DConfig& acceleration(const float movementAcceleration) {
            entity_.template get<helios::physics::motion::components::Move2DComponent<Handle_type>>()
                      ->setMovementAcceleration(movementAcceleration);

            return *this;
        }

        /**
         * @brief Enables or disables instant acceleration.
         *
         * When enabled, the entity reaches max speed immediately.
         *
         * @param useInstantAcceleration True for instant acceleration.
         *
         * @return Reference to this config for chaining.
         */
        Move2DConfig& instantAcceleration(const bool useInstantAcceleration) {
            entity_.template get<helios::physics::motion::components::Move2DComponent<Handle_type>>()
                      ->setUseInstantAcceleration(useInstantAcceleration);

            return *this;
        }

    };

}

