/** @file GameObjectPoolId.ixx
 * @brief Strongly-typed identifier for GameObjectPool instances.
 */
module;

#include <cstdint>
#include <helios/helios_config.h>
#include <string_view>
#include <unordered_map>

export module helios.engine.core.data.GameObjectPoolId;

import helios.core.algorithms;


export namespace helios::engine::core::data {

    /**
     * @brief Strongly-typed identifier for referencing a GameObjectPool.
     *
     * @details GameObjectPoolId provides type-safety when working with multiple
     * object pools. Instead of using raw integers or strings, this wrapper ensures
     * compile-time checking when accessing pools from a registry.
     *
     * The ID is hashable and comparable, making it suitable for use as a key
     * in associative containers like `std::unordered_map`.
     *
     * ## Construction
     *
     * IDs can be constructed from string literals using FNV-1a hashing:
     * ```cpp
     * constexpr GameObjectPoolId BULLET_POOL{"bullets"};
     * constexpr GameObjectPoolId ENEMY_POOL{"enemies"};
     * ```
     *
     * ## Usage
     *
     * ```cpp
     * auto* bullets = registry.pool(BULLET_POOL);
     * auto* enemies = registry.pool(ENEMY_POOL);
     * ```
     *
     * @see SpawnProfileId
     * @see SpawnRuleId
     */
    struct GameObjectPoolId {

    private:

        /**
         * @brief The underlying numeric identifier.
         */
        uint32_t id_;

        /**
         * @brief Constructs a GameObjectPoolId with the specified value.
         *
         * @param id The numeric identifier for this pool.
         */
        explicit constexpr GameObjectPoolId(const uint32_t id) noexcept
        : id_(id) {
        }

    public:

        /**
         * @brief Constructs a GameObjectPoolId from a string literal.
         *
         * @details Uses FNV-1a hashing to compute a numeric ID from the string.
         * This enables readable, compile-time constant pool identifiers.
         *
         * ```cpp
         * constexpr GameObjectPoolId BULLET_POOL{"bullets"};
         * ```
         *
         * @param str The string to hash into an ID.
         */
        explicit constexpr GameObjectPoolId(const std::string_view str) noexcept
            : GameObjectPoolId(helios::core::algorithms::fnv1a_hash(str)) {}

        /**
         * @brief Returns the underlying numeric value.
         *
         * @return The pool ID as an unsigned 32-bit integer.
         */
        [[nodiscard]] uint32_t value() const noexcept {
            return id_;
        }

        /**
         * @brief Equality comparison operator.
         */
        friend constexpr bool operator ==(GameObjectPoolId, GameObjectPoolId) noexcept = default;

        /**
         * @brief Less-than comparison operator for ordered containers.
         *
         * @param other The GameObjectPoolId to compare against.
         *
         * @return True if this ID is less than other.
         */
        constexpr bool operator<(const GameObjectPoolId& other) const noexcept {
            return id_ < other.id_;
        }

        /**
         * @brief Greater-than comparison operator for ordered containers.
         *
         * @param other The GameObjectPoolId to compare against.
         *
         * @return True if this ID is greater than other.
         */
        constexpr bool operator>(const GameObjectPoolId& other) const noexcept {
            return id_ > other.id_;
        }



    };

}

/**
 * @brief Hash specialization for GameObjectPoolId.
 *
 * Enables use as a key in std::unordered_map and std::unordered_set.
 */
template<>
struct std::hash<helios::engine::core::data::GameObjectPoolId> {
    std::size_t operator()(const  helios::engine::core::data::GameObjectPoolId& id) const noexcept {
        return id.value();
    }
};