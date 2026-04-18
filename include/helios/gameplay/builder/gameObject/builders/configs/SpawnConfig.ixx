/**
 * @file SpawnConfig.ixx
 * @brief Configuration for spawn system integration.
 */
module;

export module helios.gameplay.builder.gameObject.builders.configs.SpawnConfig;


import helios.gameplay.spawn.components;

export namespace helios::gameplay::builder::gameObject::builders::configs {

    /**
     * @brief Fluent configuration for spawn system integration.
     *
     * Provides methods for connecting GameObjects to the spawn
     * system and spawn profiles.
     */
    template<typename Entity>
    class SpawnConfig {

        /**
         * @brief Non-owning pointer to the target GameObject.
         */
        Entity gameObject_;

    public:

        using Handle_type = typename Entity::Handle_type;

        /**
         * @brief Constructs a SpawnConfig.
         *
         * @param gameObject Target GameObject to configure.
         */
        explicit SpawnConfig(Entity gameObject) : gameObject_(gameObject) {}

        /**
         * @brief Marks the entity as spawned by a spawn profile.
         *
         * Adds SpawnedByProfileComponent which tracks the spawn
         * profile that created this entity.
         *
         * @return Reference to this config for chaining.
         */
        SpawnConfig& useSpawnProfile() {
            gameObject_.template add<helios::gameplay::spawn::components::SpawnedByProfileComponent<Handle_type>>();

            return *this;
        }

        /**
         * @brief Tracks the emitter that created this entity.
         *
         * Adds EmittedByComponent which stores the handle of the entity
         * that emitted/spawned this entity (e.g., player → projectile).
         *
         * @return Reference to this config for chaining.
         */
        SpawnConfig& trackEmitter() {
            gameObject_.template add<helios::gameplay::spawn::components::EmittedByComponent<Handle_type>>();

            return *this;
        }


    };

}
