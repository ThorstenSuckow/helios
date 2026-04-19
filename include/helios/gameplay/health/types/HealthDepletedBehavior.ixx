/**
 * @file HealthDepletedBehavior.ixx
 * @brief Bitmask flags controlling the response when health reaches zero.
 */
module;

#include <cstdint>

export module helios.gameplay.health.types.HealthDepletedBehavior;


export namespace helios::gameplay::health::types {

    /**
     * @brief Bitmask enum describing what happens when an entity's health is depleted.
     *
     * Flags can be combined with the bitwise OR operator to trigger
     * multiple effects simultaneously.
     */
    enum class HealthDepletedBehavior : uint16_t {

        /**
         * @brief No action on depletion.
         */
        None    = 0,

        /**
         * @brief Queue the entity for despawning.
         */
        Despawn = 1 << 0,

        /**
         * @brief Attach a DeadTagComponent to the entity.
         */
        DeadTag = 1 << 1
    };


    /**
     * @brief Bitwise OR operator for combining flags.
     *
     * @param lhs Left-hand side flag.
     * @param rhs Right-hand side flag.
     *
     * @return Combined flags.
     */
    [[nodiscard]] constexpr HealthDepletedBehavior operator|(HealthDepletedBehavior lhs, HealthDepletedBehavior rhs) noexcept {
        return static_cast<HealthDepletedBehavior>(
            static_cast<uint16_t>(lhs) | static_cast<uint16_t>(rhs)
        );
    }


    /**
     * @brief Bitwise AND operator for testing flags.
     *
     * @param lhs Left-hand side flag.
     * @param rhs Right-hand side flag.
     *
     * @return Intersection of both flags.
     */
    [[nodiscard]] constexpr HealthDepletedBehavior operator&(HealthDepletedBehavior lhs, HealthDepletedBehavior rhs) noexcept {
        return static_cast<HealthDepletedBehavior>(
            static_cast<uint16_t>(lhs) & static_cast<uint16_t>(rhs)
        );
    }

    /**
     * @brief Tests whether a specific flag is set in a mask.
     *
     * @param mask The bitmask to test.
     * @param flag The flag to check for.
     *
     * @return True if the flag is set.
     */
    [[nodiscard]] constexpr bool hasHealthDepletedFlag(const HealthDepletedBehavior mask, const HealthDepletedBehavior flag) noexcept {
        return (mask & flag) == flag;
    }

}

