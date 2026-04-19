/**
 * @file PrefabIdComponent.ixx
 * @brief Component that stores the prefab identity of a pooled Entity.
 */
module;

export module helios.runtime.pooling.components.PrefabIdComponent;


import helios.gameplay.common.types.PrefabId;
import helios.core.types;

using namespace helios::gameplay::common::types;

export namespace helios::runtime::pooling::components {

    /**
     * @brief Component that stores the PrefabId identifying a Entity's prefab origin.
     *
     * @details PrefabIdComponent is attached to pooled Entities to record which
     * prefab template they were cloned from. The pool system uses this ID to
     * associate entities with their originating pool configuration.
     *
     * Copy construction and assignment are explicitly deleted because prefab
     * identity is intrinsic to the original template - cloned instances receive
     * their own PrefabIdComponent via move during pool initialization, not
     * through the blueprint copy path.
     *
     * @see PrefabId
     * @see EntityPoolConfig
     * @see EntityFactory::withPrefabId
     */
    template<typename THandle>
    class PrefabIdComponent  {

        /**
         * @brief The prefab identifier for this entity's template origin.
         */
        PrefabId prefabId_;

    public:

        /**
         * @brief Destructor.
         */
        ~PrefabIdComponent() = default;

        /// @brief Copy construction is deleted (prefab identity is not cloneable).
        PrefabIdComponent(const PrefabIdComponent&) = delete;

        /// @brief Copy assignment is deleted (prefab identity is not cloneable).
        PrefabIdComponent& operator=(const PrefabIdComponent&) = delete;

        /// @brief Move constructor.
        PrefabIdComponent(PrefabIdComponent&&) = default;

        /// @brief Move assignment operator.
        PrefabIdComponent& operator=(PrefabIdComponent&&) = default;

        /**
         * @brief Constructs a PrefabIdComponent with the given prefab identifier.
         *
         * @param prefabId The prefab identifier to store.
         */
        explicit PrefabIdComponent(const PrefabId prefabId) : prefabId_(prefabId) {}

        /**
         * @brief Returns the stored prefab identifier.
         *
         * @return The PrefabId identifying the prefab template.
         */
        [[nodiscard]] PrefabId prefabId() const noexcept {
            return prefabId_;
        }


    };

}