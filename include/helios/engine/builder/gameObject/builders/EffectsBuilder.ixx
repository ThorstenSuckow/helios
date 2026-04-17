/**
 * @file EffectsBuilder.ixx
 * @brief Builder for visual effects component configuration.
 */
module;

export module helios.engine.builder.gameObject.builders.EffectsBuilder;

import helios.engine.builder.gameObject.builders.configs.GfxEffectsConfig;

export namespace helios::engine::builder::gameObject::builders {

    /**
     * @brief Builder for configuring visual effects components.
     *
     * Provides factory methods for setting up effects like
     * continuous rotation (spin) on a GameObject.
     */
    template<typename Entity>
    class EffectsBuilder {

        /**
         * @brief Non-owning pointer to the target GameObject.
         */
        Entity gameObject_;

    public:

        /**
         * @brief Constructs an EffectsBuilder for the given GameObject.
         *
         * @param gameObject Target GameObject to configure.
         */
        explicit EffectsBuilder(Entity gameObject) : gameObject_(gameObject) {}

        /**
         * @brief Returns a config for graphics effects setup.
         *
         * @return GfxEffectsConfig for fluent configuration.
         */
        helios::engine::builder::gameObject::builders::configs::GfxEffectsConfig<Entity> gfx() const {
            return helios::engine::builder::gameObject::builders::configs::GfxEffectsConfig<Entity>{gameObject_};
        }


    };

}
