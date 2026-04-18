/**
 * @file AiBuilder.ixx
 * @brief Builder for AI behavior component configuration.
 */
module;

export module helios.gameplay.builder.gameObject.builders.AiBuilder;

import helios.gameplay.builder.gameObject.builders.configs.ChaseConfig;

export namespace helios::gameplay::builder::gameObject::builders {

    /**
     * @brief Builder for configuring AI behavior components.
     *
     * Provides factory methods for setting up AI behaviors
     * like chasing and seeking targets.
     */
    template<typename Entity>
    class AiBuilder {

        /**
         * @brief Non-owning pointer to the target GameObject.
         */
        Entity gameObject_;

    public:


        /**
         * @brief Constructs an AiBuilder for the given GameObject.
         *
         * @param gameObject Target GameObject to configure.
         */
        explicit AiBuilder(Entity gameObject) : gameObject_(gameObject) {}

        /**
         * @brief Returns a config for chase behavior setup.
         *
         * @return ChaseConfig for fluent configuration.
         */
        [[nodiscard]] helios::gameplay::builder::gameObject::builders::configs::ChaseConfig<Entity> chasing() const {
            return helios::gameplay::builder::gameObject::builders::configs::ChaseConfig<Entity>{gameObject_};
        }

    };

}
