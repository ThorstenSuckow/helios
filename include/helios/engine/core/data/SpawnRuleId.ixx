/**
 * @file SpawnRuleId.ixx
 * @brief Strongly-typed identifier for SpawnRule instances.
 */
module;

#include <cstdint>
#include <string_view>

export module helios.engine.core.data.SpawnRuleId;

import helios.core.algorithms;

export namespace helios::engine::core::data {

    /**
     * @brief Strongly-typed identifier for SpawnRule instances.
     *
     * @details SpawnRuleId provides type-safe identification for spawn rules,
     * which define the conditions under which entities are spawned (timing,
     * pool limits, etc.). Using a dedicated ID type prevents accidental misuse
     * and enables compile-time enforcement of correct parameter passing.
     *
     * The ID supports hashing for use in unordered containers and comparison
     * operators for ordered containers.
     *
     * ## Construction
     *
     * IDs can be constructed from string literals using FNV-1a hashing:
     * ```cpp
     * constexpr SpawnRuleId WAVE_RULE{"wave_spawn"};
     * constexpr SpawnRuleId BOSS_RULE{"boss_spawn"};
     * ```
     *
     * ## Usage
     *
     * ```cpp
     * auto rule = std::make_unique<TimerSpawnRule>(WAVE_RULE, 2.0f, 5);
     * scheduler.addRule(profileId, std::move(rule));
     * ```
     *
     * @see SpawnRule
     * @see SpawnProfileId
     * @see GameObjectPoolId
     */
    struct SpawnRuleId {

    private:

        /**
         * @brief The underlying numeric identifier.
         */
        uint32_t id_;

        /**
         * @brief Constructs a SpawnRuleId with the given numeric value.
         *
         * @param id The numeric identifier value.
         */
        explicit constexpr SpawnRuleId(uint32_t id) noexcept
            : id_(id) {
        }

    public:

        /**
         * @brief Constructs a SpawnRuleId from a string literal.
         *
         * @details Uses FNV-1a hashing to compute a numeric ID from the string.
         * This enables readable, compile-time constant rule identifiers.
         *
         * ```cpp
         * constexpr SpawnRuleId WAVE_RULE{"wave_spawn"};
         * ```
         *
         * @param str The string to hash into an ID.
         */
        explicit constexpr SpawnRuleId(const std::string_view str) noexcept
            : SpawnRuleId(helios::core::algorithms::fnv1a_hash(str)) {}

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
        friend constexpr bool operator==(SpawnRuleId, SpawnRuleId) noexcept = default;

        /**
         * @brief Less-than comparison for ordered containers.
         *
         * @param other The SpawnRuleId to compare against.
         *
         * @return True if this ID is less than the other.
         */
        constexpr bool operator<(const SpawnRuleId& other) const noexcept {
            return id_ < other.id_;
        }

        /**
         * @brief Greater-than comparison for ordered containers.
         *
         * @param other The SpawnRuleId to compare against.
         *
         * @return True if this ID is greater than the other.
         */
        constexpr bool operator>(const SpawnRuleId& other) const noexcept {
            return id_ > other.id_;
        }

    };

}

/**
 * @brief Hash specialization for SpawnRuleId.
 *
 * Enables use as a key in std::unordered_map and std::unordered_set.
 */
template<>
struct std::hash<helios::engine::core::data::SpawnRuleId> {

    /**
     * @brief Computes the hash value for a SpawnRuleId.
     *
     * @param id The SpawnRuleId to hash.
     *
     * @return The hash value based on the underlying numeric identifier.
     */
    std::size_t operator()(const helios::engine::core::data::SpawnRuleId& id) const noexcept {
        return id.value();
    }

};