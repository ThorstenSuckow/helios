/**
 * @file SpawnRuleId.ixx
 * @brief Strongly-typed identifier for SpawnRule instances.
 */
module;

#include <cstdint>
#include <compare>

export module helios.engine.core.data.SpawnRuleId;

export namespace helios::engine::core::data {

    /**
     * @brief Strongly-typed identifier for SpawnRule instances.
     *
     * @details SpawnRuleId provides type-safe identification for spawn rules,
     * which combine spawn conditions with spawn profiles to define when and
     * how entities are spawned. Using a dedicated ID type prevents accidental
     * misuse of raw integers and enables compile-time enforcement of correct
     * parameter passing.
     *
     * The ID supports hashing for use in unordered containers and comparison
     * operators for ordered containers.
     *
     * Example usage:
     * ```cpp
     * constexpr SpawnRuleId waveOneRule{1};
     * constexpr SpawnRuleId bossSpawnRule{2};
     *
     * std::unordered_map<SpawnRuleId, SpawnRule> rules;
     * rules[waveOneRule] = createWaveOneRule();
     * ```
     *
     * @see SpawnRule
     * @see SpawnProfileId
     * @see SpawnCondition
     */
    struct SpawnRuleId {

    private:

        /**
         * @brief The underlying numeric identifier.
         */
        uint32_t id_;

    public:

        /**
         * @brief Constructs a SpawnRuleId with the given numeric value.
         *
         * @param id The numeric identifier value.
         */
        explicit constexpr SpawnRuleId(uint32_t id) noexcept
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