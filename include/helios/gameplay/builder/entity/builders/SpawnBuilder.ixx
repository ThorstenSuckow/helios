/**
 * @file SpawnBuilder.ixx
 * @brief Builder for spawn system component configuration.
 */
module;

export module helios.gameplay.builder.entity.builders.SpawnBuilder;

import helios.gameplay.builder.entity.builders.configs.SpawnConfig;

export namespace helios::gameplay::builder::entity::builders {

    /**
     * @brief Builder for configuring spawn-related components.
     *
     * Provides factory methods for integrating Entities
     * with the spawn system and spawn profiles.
     */
    template<typename Entity>
    class SpawnBuilder {

        /**
         * @brief Non-owning pointer to the target Entity.
         */
        Entity entity_;

    public:

        /**
         * @brief Constructs a SpawnBuilder for the given Entity.
         *
         * @param entity Target Entity to configure.
         */
        explicit SpawnBuilder(Entity entity) : entity_(entity) {}

        /**
         * @brief Returns a config for spawn profile setup.
         *
         * @return SpawnConfig for fluent configuration.
         */
        helios::gameplay::builder::entity::builders::configs::SpawnConfig<Entity> spawn() {
            return helios::gameplay::builder::entity::builders::configs::SpawnConfig<Entity>{entity_};
        }


    };

}
