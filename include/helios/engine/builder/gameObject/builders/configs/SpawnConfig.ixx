/**
 * @file SpawnConfig.ixx
 * @brief Configuration for spawn system integration.
 */
module;

export module helios.engine.builder.gameObject.builders.configs.SpawnConfig;

import helios.engine.ecs.GameObject;

import helios.engine.mechanics.spawn.components;

export namespace helios::engine::builder::gameObject::builders::configs {

    /**
     * @brief Fluent configuration for spawn system integration.
     *
     * Provides methods for connecting GameObjects to the spawn
     * system and spawn profiles.
     */
    class SpawnConfig {

        /**
         * @brief Non-owning pointer to the target GameObject.
         */
        helios::engine::ecs::GameObject* gameObject_;

    public:

        /**
         * @brief Constructs a SpawnConfig.
         *
         * @param gameObject Target GameObject to configure.
         */
        explicit SpawnConfig(helios::engine::ecs::GameObject* gameObject) : gameObject_(gameObject) {}

        /**
         * @brief Marks the entity as spawned by a spawn profile.
         *
         * Adds SpawnedByProfileComponent which tracks the spawn
         * profile that created this entity.
         *
         * @return Reference to this config for chaining.
         */
        SpawnConfig& useSpawnProfile() {
            gameObject_->add<helios::engine::mechanics::spawn::components::SpawnedByProfileComponent>();

            return *this;
        }

        /**
         * @brief Tracks the emitter that created this entity.
         *
         * Adds EmittedByComponent which stores the GUID of the entity
         * that emitted/spawned this entity (e.g., player → projectile).
         *
         * @return Reference to this config for chaining.
         */
        SpawnConfig& trackEmitter() {
            gameObject_->add<helios::engine::mechanics::spawn::components::EmittedByComponent>();

            return *this;
        }


    };

}

