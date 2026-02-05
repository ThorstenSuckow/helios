/**
 * @file ScorePoolId.ixx
 * @brief Strongly-typed identifier for score pools.
 */
module;

#include <cstdint>
#include <string_view>

export module helios.engine.core.data.ScorePoolId;

import helios.core.algorithms;
import helios.core.types;

export namespace helios::engine::core::data {

    /**
     * @brief Strongly-typed identifier for score pools.
     *
     * Uses FNV-1a string hashing to generate unique identifiers at compile time.
     * Enables type-safe referencing of score pools by name.
     */
    struct ScorePoolId {

    private:

        /**
         * @brief The hashed identifier value.
         */
        uint32_t id_{};

        /**
         * @brief Private constructor from raw hash value.
         *
         * @param id The pre-computed hash value.
         */
        explicit constexpr ScorePoolId(const uint32_t id) noexcept
            : id_(id) {
        }

    public:

        /**
         * @brief Constructs a ScorePoolId from a string name.
         *
         * Uses FNV-1a hashing to convert the string to a 32-bit identifier.
         * This constructor is `constexpr`, allowing compile-time evaluation.
         *
         * @param str The viewport name (e.g., "Ui", "Game").
         */
        explicit constexpr ScorePoolId(const std::string_view str) noexcept
            : ScorePoolId(helios::core::algorithms::fnv1a_hash(str)) {}

        explicit constexpr ScorePoolId(helios::core::types::no_init_t) {};

        /**
         * @brief Returns the raw identifier value.
         *
         * @return The 32-bit hash value.
         */
        [[nodiscard]] uint32_t value() const noexcept {
            return id_;
        }


        [[nodiscard]] explicit operator bool() const noexcept {
            return id_ != 0;
        }

        /**
         * @brief Equality comparison operator.
         */
        friend constexpr bool operator==(ScorePoolId, ScorePoolId) noexcept = default;

        /**
         * @brief Less-than comparison for ordered containers.
         *
         * @param other The ScorePoolId to compare against.
         *
         * @return `true` if this id is less than `other`.
         */
        constexpr bool operator<(const ScorePoolId& other) const noexcept {
            return id_ < other.id_;
        }

        /**
         * @brief Greater-than comparison for ordered containers.
         *
         * @param other The ScorePoolId to compare against.
         *
         * @return `true` if this id is greater than `other`.
         */
        constexpr bool operator>(const ScorePoolId& other) const noexcept {
            return id_ > other.id_;
        }

    };

}

/**
 * @brief Hash specialization for `ScorePoolId` to enable use in unordered containers.
 */
template<>
struct std::hash<helios::engine::core::data::ScorePoolId> {

    /**
     * @brief Computes the hash value for a ScorePoolId.
     *
     * @param id The ScorePoolId to hash.
     *
     * @return The hash value (same as the internal id).
     */
    std::size_t operator()(const helios::engine::core::data::ScorePoolId& id) const noexcept {
        return id.value();
    }

};