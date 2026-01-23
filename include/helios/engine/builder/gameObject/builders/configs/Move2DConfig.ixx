/**
 * @file Move2DConfig.ixx
 * @brief Configuration for 2D movement components.
 */
module;

export module helios.engine.builder.gameObject.builders.configs.Move2DConfig;

import helios.engine.ecs.GameObject;
import helios.engine.modules.physics;
import helios.engine.modules.spatial;

export namespace helios::engine::builder::gameObject::builders::configs {

    /**
     * @brief Fluent configuration for 2D movement behavior.
     *
     * Automatically adds required components:
     * - Move2DComponent
     * - ComposeTransformComponent
     * - DirectionComponent
     * - TranslationStateComponent
     */
    class Move2DConfig {

        /**
         * @brief Non-owning pointer to the target GameObject.
         */
        helios::engine::ecs::GameObject* gameObject_;

    public:

        /**
         * @brief Constructs a Move2DConfig and adds required components.
         *
         * @param gameObject Target GameObject to configure.
         */
        explicit Move2DConfig(helios::engine::ecs::GameObject* gameObject) : gameObject_(gameObject) {

            gameObject_->add<helios::engine::modules::physics::motion::components::Move2DComponent>();

            gameObject_->getOrAdd<helios::engine::modules::spatial::transform::components::ComposeTransformComponent>();
            gameObject_->getOrAdd<helios::engine::modules::physics::motion::components::DirectionComponent>();
            gameObject_->getOrAdd<helios::engine::modules::spatial::transform::components::TranslationStateComponent>();

        }

        /**
         * @brief Sets the maximum movement speed.
         *
         * @param movementSpeed Speed in units per second.
         *
         * @return Reference to this config for chaining.
         */
        Move2DConfig& speed(const float movementSpeed) {
            gameObject_->get<helios::engine::modules::physics::motion::components::Move2DComponent>()
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
            gameObject_->get<helios::engine::modules::physics::motion::components::Move2DComponent>()
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
            gameObject_->get<helios::engine::modules::physics::motion::components::Move2DComponent>()
                      ->setUseInstantAcceleration(useInstantAcceleration);

            return *this;
        }

    };

}

