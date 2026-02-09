/**
 * @file SpawnBuilder.ixx
 * @brief Builder for spawn system component configuration.
 */
module;

export module helios.engine.builder.gameObject.builders.SpawnBuilder;

import helios.engine.ecs.GameObject;
import helios.engine.builder.gameObject.builders.configs.SpawnConfig;

export namespace helios::engine::builder::gameObject::builders {

    /**
     * @brief Builder for configuring spawn-related components.
     *
     * Provides factory methods for integrating GameObjects
     * with the spawn system and spawn profiles.
     */
    class SpawnBuilder {

        /**
         * @brief Non-owning pointer to the target GameObject.
         */
        helios::engine::ecs::GameObject gameObject_;

    public:

        /**
         * @brief Constructs a SpawnBuilder for the given GameObject.
         *
         * @param gameObject Target GameObject to configure.
         */
        explicit SpawnBuilder(helios::engine::ecs::GameObject gameObject) : gameObject_(gameObject) {}

        /**
         * @brief Returns a config for spawn profile setup.
         *
         * @return SpawnConfig for fluent configuration.
         */
        helios::engine::builder::gameObject::builders::configs::SpawnConfig spawn() {
            return helios::engine::builder::gameObject::builders::configs::SpawnConfig{gameObject_};
        }


    };

}
