/**
 * @file UiTransformBuilder.ixx
 * @brief Builder for UI transform component configuration.
 */
module;

export module helios.gameplay.builder.entity.builders.UiTransformBuilder;

import helios.gameplay.builder.entity.builders.configs.UiTransformConfig;

export namespace helios::gameplay::builder::entity::builders {

    /**
     * @brief Builder for configuring UI transform components.
     *
     * Provides methods to create config objects for viewport-relative
     * positioning and UI layout.
     */
    template<typename Entity>
    class UiTransformBuilder {

        /**
         * @brief Non-owning pointer to the target Entity.
         */
        Entity entity_;

    public:

        /**
         * @brief Constructs a UiTransformBuilder for the given Entity.
         *
         * @param entity The target Entity to configure.
         */
        explicit UiTransformBuilder(Entity entity) : entity_(entity) {}

        /**
         * @brief Returns a config object for UiTransformComponent.
         *
         * @return UiTransformConfig for fluent configuration.
         */
        helios::gameplay::builder::entity::builders::configs::UiTransformConfig<Entity> transform() const {
            return helios::gameplay::builder::entity::builders::configs::UiTransformConfig<Entity>{entity_};
        }

    };

}
