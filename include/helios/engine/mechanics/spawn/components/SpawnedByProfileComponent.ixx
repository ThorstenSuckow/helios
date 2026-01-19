/**
 * @file SpawnedByProfileComponent.ixx
 * @brief Component that tracks which spawn profile created a GameObject.
 */
module;

export module helios.engine.mechanics.spawn.components.SpawnedByProfileComponent;

import helios.engine.ecs.CloneableComponent;
import helios.engine.core.data.SpawnProfileId;

export namespace helios::engine::mechanics::spawn::components {

    /**
     * @brief Component that stores the spawn profile ID that created this GameObject.
     *
     * @details SpawnedByProfileComponent is attached to pooled GameObjects to track
     * which spawn profile they belong to. This enables the despawn system to route
     * release requests to the correct pool manager.
     *
     * The component is cloneable, allowing prefab-based pooling where the profile ID
     * is set after cloning during the acquire phase.
     *
     * Example usage:
     * ```cpp
     * // During prefab setup
     * auto prefab = std::make_unique<GameObject>();
     * prefab->add<SpawnedByProfileComponent>();
     *
     * // During spawn (in SpawnManager)
     * auto* comp = entity->get<SpawnedByProfileComponent>();
     * comp->setSpawnProfileId(profileId);
     *
     * // During despawn
     * auto profileId = entity->get<SpawnedByProfileComponent>()->spawnProfileId();
     * poolManager.release(profileId, entity->guid());
     * ```
     *
     * @see SpawnProfileId
     * @see GameObjectPoolManager
     */
    class SpawnedByProfileComponent : public helios::engine::ecs::CloneableComponent<SpawnedByProfileComponent> {

        /**
         * @brief The spawn profile ID that created this entity.
         *
         * Default-initialized to 0, which represents an invalid/unassigned profile.
         */
        helios::engine::core::data::SpawnProfileId spawnProfileId_{0};

    public:

        /**
         * @brief Default constructor.
         */
        SpawnedByProfileComponent() = default;

        /**
         * @brief Copy constructor for cloning.
         *
         * @param other The component to copy from.
         */
        explicit SpawnedByProfileComponent(const SpawnedByProfileComponent& other)
            : spawnProfileId_(other.spawnProfileId_) {}

        /**
         * @brief Returns the spawn profile ID.
         *
         * @return The SpawnProfileId assigned to this entity.
         */
        [[nodiscard]] helios::engine::core::data::SpawnProfileId spawnProfileId() const noexcept {
            return spawnProfileId_;
        }

        /**
         * @brief Sets the spawn profile ID.
         *
         * @details Called by the spawn manager after acquiring an entity from the pool
         * to associate it with the correct profile for later despawning.
         *
         * @param spawnProfileId The profile ID to assign.
         */
        void setSpawnProfileId(const helios::engine::core::data::SpawnProfileId& spawnProfileId) noexcept {
            spawnProfileId_ = spawnProfileId;
        }

    };

}