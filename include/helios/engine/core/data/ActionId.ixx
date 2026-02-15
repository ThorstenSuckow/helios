/**
 * @file ActionId.ixx
 * @brief Strongly-typed identifier for UI actions.
 */
module;

#include <cstdint>
#include <string_view>

export module helios.engine.core.data.ActionId;

import helios.core.algorithms;
import helios.core.types;

export namespace helios::engine::core::data {

    /**
     * @brief Strongly-typed identifier for UI actions.
     *
     * Uses FNV-1a hashing to convert string literals to unique IDs at compile time.
     * Supports comparison and hashing for use in containers.
     */
    struct ActionId {

    private:

        /**
         * @brief The hashed identifier value.
         */
        uint32_t id_{0};

        /**
         * @brief Constructs an ActionId from a raw hash value.
         *
         * @param id The hash value.
         */
        explicit constexpr ActionId(const uint32_t id) noexcept
            : id_(id) {
        }

    public:

        /**
         * @brief Default constructor. Initializes to zero.
         */
        ActionId() = default;

        /**
         * @brief Constructs an ActionId from a string literal.
         *
         * @param str The string to hash.
         */
        explicit constexpr ActionId(const std::string_view str) noexcept
            : ActionId(helios::core::algorithms::fnv1a_hash(str)) {}

        /**
         * @brief Constructs an uninitialized ActionId.
         */
        explicit constexpr ActionId(helios::core::types::no_init_t) {};

        /**
         * @brief Returns the raw identifier value.
         *
         * @return The hash value.
         */
        [[nodiscard]] uint32_t value() const noexcept {
            return id_;
        }

        /**
         * @brief Equality comparison.
         *
         * @return True if both IDs are equal.
         */
        friend constexpr bool operator==(ActionId, ActionId) noexcept = default;

        /**
         * @brief Less-than comparison.
         *
         * @param other The other ActionId to compare.
         *
         * @return True if this ID is less than other.
         */
        constexpr bool operator<(const ActionId& other) const noexcept {
            return id_ < other.id_;
        }

        /**
         * @brief Greater-than comparison.
         *
         * @param other The other ActionId to compare.
         *
         * @return True if this ID is greater than other.
         */
        constexpr bool operator>(const ActionId& other) const noexcept {
            return id_ > other.id_;
        }

    };

}

/**
 * @brief Hash specialization for ActionId.
 */
template<>
struct std::hash<helios::engine::core::data::ActionId> {

    /**
     * @brief Computes hash for ActionId.
     *
     * @param id The ActionId to hash.
     *
     * @return The hash value.
     */
    std::size_t operator()(const helios::engine::core::data::ActionId& id) const noexcept {
        return id.value();
    }

};