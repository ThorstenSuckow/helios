/**
 * @file UiTransformBuilder.ixx
 * @brief Builder for UI transform component configuration.
 */
module;

export module helios.engine.builder.gameObject.builders.UiTransformBuilder;

import helios.engine.ecs.GameObject;
import helios.engine.builder.gameObject.builders.configs.UiTransformConfig;

export namespace helios::engine::builder::gameObject::builders {

    /**
     * @brief Builder for configuring UI transform components.
     *
     * Provides methods to create config objects for viewport-relative
     * positioning and UI layout.
     */
    class UiTransformBuilder {

        /**
         * @brief Non-owning pointer to the target GameObject.
         */
        helios::engine::ecs::GameObject gameObject_;

    public:

        /**
         * @brief Constructs a UiTransformBuilder for the given GameObject.
         *
         * @param gameObject The target GameObject to configure.
         */
        explicit UiTransformBuilder(helios::engine::ecs::GameObject gameObject) : gameObject_(gameObject) {}

        /**
         * @brief Returns a config object for UiTransformComponent.
         *
         * @return UiTransformConfig for fluent configuration.
         */
        helios::engine::builder::gameObject::builders::configs::UiTransformConfig transform() const {
            return helios::engine::builder::gameObject::builders::configs::UiTransformConfig{gameObject_};
        }

    };

}
