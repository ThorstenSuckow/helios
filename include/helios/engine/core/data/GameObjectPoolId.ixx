/** @file GameObjectPoolId.ixx
 * @brief Strongly-typed identifier for GameObjectPool instances.
 */
module;

#include <cstdint>
#include <compare>

export module helios.engine.core.data.GameObjectPoolId;



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
     * Example usage:
     * ```cpp
     * constexpr GameObjectPoolId BULLET_POOL{1};
     * constexpr GameObjectPoolId ENEMY_POOL{2};
     *
     * auto* bullets = registry.pool(BULLET_POOL);
     * auto* enemies = registry.pool(ENEMY_POOL);
     * ```
     */
    struct GameObjectPoolId {

    private:

        /**
         * @brief The underlying numeric identifier.
         */
        uint32_t id_;

    public:

        /**
         * @brief Constructs a GameObjectPoolId with the specified value.
         *
         * @param id The numeric identifier for this pool.
         */
        explicit constexpr GameObjectPoolId(uint32_t id) noexcept
        : id_(id) {}

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