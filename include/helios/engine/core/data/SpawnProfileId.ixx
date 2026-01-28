/**
 * @file SpawnProfileId.ixx
 * @brief Strongly-typed identifier for SpawnProfile instances.
 */
module;

#include <cstdint>
#include <string_view>

export module helios.engine.core.data.SpawnProfileId;

import helios.core.types;
import helios.core.algorithms;


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
     * ## Construction
     *
     * IDs can be constructed from string literals using FNV-1a hashing:
     * ```cpp
     * constexpr SpawnProfileId ENEMY_PROFILE{"enemy_spawn"};
     * constexpr SpawnProfileId POWERUP_PROFILE{"powerup_spawn"};
     * ```
     *
     * Uninitialized IDs for deferred assignment:
     * ```cpp
     * SpawnProfileId profileId{helios::core::types::no_init};
     * profileId = SpawnProfileId{"assigned_later"};
     * ```
     *
     * ## Usage
     *
     * ```cpp
     * std::unordered_map<SpawnProfileId, SpawnProfile> profiles;
     * profiles[ENEMY_PROFILE] = createEnemyProfile();
     * ```
     *
     * @see SpawnProfile
     * @see SpawnRuleId
     * @see GameObjectPoolId
     */
    struct SpawnProfileId {

    private:

        /**
         * @brief The underlying numeric identifier.
         */
        uint32_t id_{};

        /**
         * @brief Constructs a SpawnProfileId with the given numeric value.
         *
         * @param id The numeric identifier value.
         */
        explicit constexpr SpawnProfileId(const uint32_t id) noexcept
            : id_(id) {}

    public:

        /**
         * @brief Constructs an uninitialized SpawnProfileId.
         *
         * @details Use this constructor when the ID will be assigned later.
         * Reading from an uninitialized ID is undefined behavior.
         *
         * @see helios::core::types::no_init
         */
        explicit constexpr SpawnProfileId(helios::core::types::no_init_t) {}

        /**
         * @brief Constructs a SpawnProfileId from a string literal.
         *
         * @details Uses FNV-1a hashing to compute a numeric ID from the string.
         * This enables readable, compile-time constant profile identifiers.
         *
         * ```cpp
         * constexpr SpawnProfileId ENEMY_PROFILE{"enemy_spawn"};
         * ```
         *
         * @param str The string to hash into an ID.
         */
        explicit constexpr SpawnProfileId(const std::string_view str) noexcept
         : SpawnProfileId(helios::core::algorithms::fnv1a_hash(str)) {}


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