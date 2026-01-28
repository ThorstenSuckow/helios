/**
 * @file TransformBuilder.ixx
 * @brief Builder for spatial transform component configuration.
 */
module;

export module helios.engine.builder.gameObject.builders.TransformBuilder;

import helios.engine.ecs.GameObject;
import helios.engine.builder.gameObject.builders.configs.TransformConfig;

export namespace helios::engine::builder::gameObject::builders {

    /**
     * @brief Builder for configuring spatial transform components.
     *
     * Provides factory methods for setting up translation,
     * rotation, and scale on a GameObject.
     */
    class TransformBuilder {

        /**
         * @brief Non-owning pointer to the target GameObject.
         */
        helios::engine::ecs::GameObject* gameObject_;

    public:

        /**
         * @brief Constructs a TransformBuilder for the given GameObject.
         *
         * @param gameObject Target GameObject to configure.
         */
        explicit TransformBuilder(helios::engine::ecs::GameObject* gameObject) : gameObject_(gameObject) {}

        /**
         * @brief Returns a config for transform setup.
         *
         * @return TransformConfig for fluent configuration.
         */
        helios::engine::builder::gameObject::builders::configs::TransformConfig transform() const {
            return helios::engine::builder::gameObject::builders::configs::TransformConfig{gameObject_};
        }


    };

}
