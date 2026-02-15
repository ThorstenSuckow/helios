/**
 * @file MenuId.ixx
 * @brief Strongly-typed identifier for UI menus.
 */
module;

#include <cstdint>
#include <string_view>

export module helios.engine.core.data.MenuId;

import helios.core.algorithms;
import helios.core.types;

export namespace helios::engine::core::data {

    /**
     * @brief Strongly-typed identifier for UI menus.
     *
     * Uses FNV-1a hashing to convert string literals to unique IDs at compile time.
     * Supports comparison and hashing for use in containers.
     */
    struct MenuId {

    private:

        /**
         * @brief The hashed identifier value.
         */
        uint32_t id_{0};

        /**
         * @brief Constructs a MenuId from a raw hash value.
         *
         * @param id The hash value.
         */
        explicit constexpr MenuId(const uint32_t id) noexcept
            : id_(id) {
        }

    public:

        /**
         * @brief Default constructor. Initializes to zero.
         */
        MenuId() = default;

        /**
         * @brief Constructs a MenuId from a string literal.
         *
         * @param str The string to hash.
         */
        explicit constexpr MenuId(const std::string_view str) noexcept
            : MenuId(helios::core::algorithms::fnv1a_hash(str)) {}

        /**
         * @brief Constructs an uninitialized MenuId.
         */
        explicit constexpr MenuId(helios::core::types::no_init_t) {};

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
        friend constexpr bool operator==(MenuId, MenuId) noexcept = default;

        /**
         * @brief Less-than comparison.
         *
         * @param other The other MenuId to compare.
         *
         * @return True if this ID is less than other.
         */
        constexpr bool operator<(const MenuId& other) const noexcept {
            return id_ < other.id_;
        }

        /**
         * @brief Greater-than comparison.
         *
         * @param other The other MenuId to compare.
         *
         * @return True if this ID is greater than other.
         */
        constexpr bool operator>(const MenuId& other) const noexcept {
            return id_ > other.id_;
        }

    };

}

/**
 * @brief Hash specialization for MenuId.
 */
template<>
struct std::hash<helios::engine::core::data::MenuId> {

    /**
     * @brief Computes hash for MenuId.
     *
     * @param id The MenuId to hash.
     *
     * @return The hash value.
     */
    std::size_t operator()(const helios::engine::core::data::MenuId& id) const noexcept {
        return id.value();
    }

};