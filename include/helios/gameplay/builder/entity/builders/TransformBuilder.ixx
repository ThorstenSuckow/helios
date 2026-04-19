/**
 * @file TransformBuilder.ixx
 * @brief Builder for spatial transform component configuration.
 */
module;

export module helios.gameplay.builder.entity.builders.TransformBuilder;

import helios.gameplay.builder.entity.builders.configs.TransformConfig;

export namespace helios::gameplay::builder::entity::builders {

    /**
     * @brief Builder for configuring spatial transform components.
     *
     * Provides factory methods for setting up translation,
     * rotation, and scale on a Entity.
     */
    template<typename Entity>
    class TransformBuilder {

        /**
         * @brief Non-owning pointer to the target Entity.
         */
        Entity entity_;

    public:

        /**
         * @brief Constructs a TransformBuilder for the given Entity.
         *
         * @param entity Target Entity to configure.
         */
        explicit TransformBuilder(Entity entity) : entity_(entity) {}

        /**
         * @brief Returns a config for transform setup.
         *
         * @return TransformConfig for fluent configuration.
         */
        helios::gameplay::builder::entity::builders::configs::TransformConfig<Entity> transform() const {
            return helios::gameplay::builder::entity::builders::configs::TransformConfig<Entity>{entity_};
        }


    };

}
