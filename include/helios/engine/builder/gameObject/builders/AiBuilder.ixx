/**
 * @file AiBuilder.ixx
 * @brief Builder for AI behavior component configuration.
 */
module;

export module helios.engine.builder.gameObject.builders.AiBuilder;

import helios.engine.ecs.GameObject;
import helios.engine.builder.gameObject.builders.configs.ChaseConfig;

export namespace helios::engine::builder::gameObject::builders {

    /**
     * @brief Builder for configuring AI behavior components.
     *
     * Provides factory methods for setting up AI behaviors
     * like chasing and seeking targets.
     */
    class AiBuilder {

        /**
         * @brief Non-owning pointer to the target GameObject.
         */
        helios::engine::ecs::GameObject* gameObject_;

    public:

        /**
         * @brief Constructs an AiBuilder for the given GameObject.
         *
         * @param gameObject Target GameObject to configure.
         */
        explicit AiBuilder(helios::engine::ecs::GameObject* gameObject) : gameObject_(gameObject) {}

        /**
         * @brief Returns a config for chase behavior setup.
         *
         * @return ChaseConfig for fluent configuration.
         */
        [[nodiscard]] helios::engine::builder::gameObject::builders::configs::ChaseConfig chasing() const {
            return helios::engine::builder::gameObject::builders::configs::ChaseConfig{gameObject_};
        }

    };

}
