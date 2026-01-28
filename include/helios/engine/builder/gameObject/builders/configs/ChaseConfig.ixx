/**
 * @file ChaseConfig.ixx
 * @brief Configuration for AI chase behavior.
 */
module;

export module helios.engine.builder.gameObject.builders.configs.ChaseConfig;

import helios.engine.ecs.GameObject;

import helios.util.Guid;

import helios.engine.modules.ai.components.ChaseComponent;

export namespace helios::engine::builder::gameObject::builders::configs {

    /**
     * @brief Fluent configuration for AI chase behavior.
     *
     * Automatically adds ChaseComponent and provides methods
     * for setting the chase target and seek interval.
     */
    class ChaseConfig {

        /**
         * @brief Non-owning pointer to the target GameObject.
         */
        helios::engine::ecs::GameObject* gameObject_;

    public:

        /**
         * @brief Constructs a ChaseConfig and adds ChaseComponent.
         *
         * @param gameObject Target GameObject to configure.
         */
        explicit ChaseConfig(helios::engine::ecs::GameObject* gameObject) : gameObject_(gameObject) {
            gameObject_->add<helios::engine::modules::ai::components::ChaseComponent>();
        }

        /**
         * @brief Sets the target entity to chase.
         *
         * @param guid The GUID of the target GameObject.
         *
         * @return Reference to this config for chaining.
         */
        ChaseConfig& target(const helios::util::Guid guid) {
            gameObject_->getOrAdd<helios::engine::modules::ai::components::ChaseComponent>()
                        .setTarget(guid);

            return *this;
        }

        /**
         * @brief Sets the interval between target position updates.
         *
         * @param interval Time in seconds between seek updates.
         *
         * @return Reference to this config for chaining.
         */
        ChaseConfig& seekInterval(const float interval) {
            gameObject_->getOrAdd<helios::engine::modules::ai::components::ChaseComponent>()
                        .setCooldown(interval);

            return *this;
        }

    };

}

