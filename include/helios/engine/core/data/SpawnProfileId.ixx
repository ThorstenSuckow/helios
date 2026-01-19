/**
 * @file SpawnProfileId.ixx
 * @brief Strongly-typed identifier for SpawnProfile instances.
 */
module;

#include <cstdint>
#include <compare>

export module helios.engine.core.data.SpawnProfileId;

export namespace helios::engine::core::data {

    /**
     * @brief Strongly-typed identifier for SpawnProfile instances.
     *
     * @details SpawnProfileId provides type-safe identification for spawn profiles,
     * which define how entities are spawned (pool reference, placer, initializer).
     * Using a dedicated ID type prevents accidental misuse of raw integers and
     * enables compile-time enforcement of correct parameter passing.
     *
     * The ID supports hashing for use in unordered containers and comparison
     * operators for ordered containers.
     *
     * Example usage:
     * ```cpp
     * constexpr SpawnProfileId enemyProfile{1};
     * constexpr SpawnProfileId powerUpProfile{2};
     *
     * std::unordered_map<SpawnProfileId, SpawnProfile> profiles;
     * profiles[enemyProfile] = createEnemyProfile();
     * ```
     *
     * @see SpawnProfile
     * @see SpawnRuleId
     */
    struct SpawnProfileId {

    private:

        /**
         * @brief The underlying numeric identifier.
         */
        uint32_t id_;

    public:

        /**
         * @brief Constructs a SpawnProfileId with the given numeric value.
         *
         * @param id The numeric identifier value.
         */
        explicit constexpr SpawnProfileId(uint32_t id) noexcept
            : id_(id) {}

        /**
         * @brief Returns the underlying numeric value.
         *
         * @return The raw identifier value.
         */
        [[nodiscard]] uint32_t value() const noexcept {
            return id_;
        }

        /**
         * @brief Default equality comparison.
         */
        friend constexpr bool operator==(SpawnProfileId, SpawnProfileId) noexcept = default;

        /**
         * @brief Less-than comparison for ordered containers.
         *
         * @param other The SpawnProfileId to compare against.
         *
         * @return True if this ID is less than the other.
         */
        constexpr bool operator<(const SpawnProfileId& other) const noexcept {
            return id_ < other.id_;
        }

        /**
         * @brief Greater-than comparison for ordered containers.
         *
         * @param other The SpawnProfileId to compare against.
         *
         * @return True if this ID is greater than the other.
         */
        constexpr bool operator>(const SpawnProfileId& other) const noexcept {
            return id_ > other.id_;
        }

    };

}

/**
 * @brief Hash specialization for SpawnProfileId.
 *
 * Enables use as a key in std::unordered_map and std::unordered_set.
 */
template<>
struct std::hash<helios::engine::core::data::SpawnProfileId> {

    /**
     * @brief Computes the hash value for a SpawnProfileId.
     *
     * @param id The SpawnProfileId to hash.
     *
     * @return The hash value based on the underlying numeric identifier.
     */
    std::size_t operator()(const helios::engine::core::data::SpawnProfileId& id) const noexcept {
        return id.value();
    }

};