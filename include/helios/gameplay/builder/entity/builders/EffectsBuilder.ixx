/**
 * @file EffectsBuilder.ixx
 * @brief Builder for visual effects component configuration.
 */
module;

export module helios.gameplay.builder.entity.builders.EffectsBuilder;

import helios.gameplay.builder.entity.builders.configs.GfxEffectsConfig;

export namespace helios::gameplay::builder::entity::builders {

    /**
     * @brief Builder for configuring visual effects components.
     *
     * Provides factory methods for setting up effects like
     * continuous rotation (spin) on a Entity.
     */
    template<typename Entity>
    class EffectsBuilder {

        /**
         * @brief Non-owning pointer to the target Entity.
         */
        Entity entity_;

    public:

        /**
         * @brief Constructs an EffectsBuilder for the given Entity.
         *
         * @param entity Target Entity to configure.
         */
        explicit EffectsBuilder(Entity entity) : entity_(entity) {}

        /**
         * @brief Returns a config for graphics effects setup.
         *
         * @return GfxEffectsConfig for fluent configuration.
         */
        helios::gameplay::builder::entity::builders::configs::GfxEffectsConfig<Entity> gfx() const {
            return helios::gameplay::builder::entity::builders::configs::GfxEffectsConfig<Entity>{entity_};
        }


    };

}
