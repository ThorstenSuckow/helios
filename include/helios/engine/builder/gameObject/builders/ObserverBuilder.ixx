/**
 * @file ObserverBuilder.ixx
 * @brief Builder for observer component configuration.
 */
module;

export module helios.engine.builder.gameObject.builders.ObserverBuilder;

import helios.engine.builder.gameObject.builders.configs.ObserverConfig;

export namespace helios::engine::builder::gameObject::builders {

    /**
     * @brief Builder for configuring observer components.
     *
     * Provides methods to create config objects for observer components
     * that bind to data sources like score pools.
     */
    template<typename Entity>
    class ObserverBuilder {

        /**
         * @brief Non-owning pointer to the target GameObject.
         */
        Entity gameObject_;

    public:

        /**
         * @brief Constructs an ObserverBuilder for the given GameObject.
         *
         * @param gameObject The target GameObject to configure.
         */
        explicit ObserverBuilder(Entity gameObject) : gameObject_(gameObject) {}

        /**
         * @brief Returns a config object for observer components.
         *
         * @return ObserverConfig for fluent configuration.
         */
        [[nodiscard]] helios::engine::builder::gameObject::builders::configs::ObserverConfig<Entity> observe() const {
            return helios::engine::builder::gameObject::builders::configs::ObserverConfig<Entity>{gameObject_};
        }

    };

}
