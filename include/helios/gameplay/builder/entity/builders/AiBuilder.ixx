/**
 * @file AiBuilder.ixx
 * @brief Builder for AI behavior component configuration.
 */
module;

export module helios.gameplay.builder.entity.builders.AiBuilder;

import helios.gameplay.builder.entity.builders.configs.ChaseConfig;

export namespace helios::gameplay::builder::entity::builders {

    /**
     * @brief Builder for configuring AI behavior components.
     *
     * Provides factory methods for setting up AI behaviors
     * like chasing and seeking targets.
     */
    template<typename Entity>
    class AiBuilder {

        /**
         * @brief Non-owning pointer to the target Entity.
         */
        Entity entity_;

    public:


        /**
         * @brief Constructs an AiBuilder for the given Entity.
         *
         * @param entity Target Entity to configure.
         */
        explicit AiBuilder(Entity entity) : entity_(entity) {}

        /**
         * @brief Returns a config for chase behavior setup.
         *
         * @return ChaseConfig for fluent configuration.
         */
        [[nodiscard]] helios::gameplay::builder::entity::builders::configs::ChaseConfig<Entity> chasing() const {
            return helios::gameplay::builder::entity::builders::configs::ChaseConfig<Entity>{entity_};
        }

    };

}
