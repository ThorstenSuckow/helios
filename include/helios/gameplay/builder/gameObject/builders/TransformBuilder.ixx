/**
 * @file TransformBuilder.ixx
 * @brief Builder for spatial transform component configuration.
 */
module;

export module helios.gameplay.builder.gameObject.builders.TransformBuilder;

import helios.gameplay.builder.gameObject.builders.configs.TransformConfig;

export namespace helios::gameplay::builder::gameObject::builders {

    /**
     * @brief Builder for configuring spatial transform components.
     *
     * Provides factory methods for setting up translation,
     * rotation, and scale on a GameObject.
     */
    template<typename Entity>
    class TransformBuilder {

        /**
         * @brief Non-owning pointer to the target GameObject.
         */
        Entity gameObject_;

    public:

        /**
         * @brief Constructs a TransformBuilder for the given GameObject.
         *
         * @param gameObject Target GameObject to configure.
         */
        explicit TransformBuilder(Entity gameObject) : gameObject_(gameObject) {}

        /**
         * @brief Returns a config for transform setup.
         *
         * @return TransformConfig for fluent configuration.
         */
        helios::gameplay::builder::gameObject::builders::configs::TransformConfig<Entity> transform() const {
            return helios::gameplay::builder::gameObject::builders::configs::TransformConfig<Entity>{gameObject_};
        }


    };

}
