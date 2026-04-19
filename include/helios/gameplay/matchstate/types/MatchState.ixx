/**
 * @file MatchState.ixx
 * @brief Match state enumeration and bitmask operations.
 */
module;

#include <cstdint>
#include <cstddef>

export module helios.gameplay.matchstate.types.MatchState;



export namespace helios::gameplay::matchstate::types {

    /**
     * @brief Underlying type for MatchState bitmask values.
     */
    using MatchStateType = uint16_t;

    constexpr size_t MatchStateTypeSize = 16;

    /**
     * @brief Enumeration of possible match states.
     *
     * States can be combined using bitwise operators for masking.
     */
    enum class MatchState : MatchStateType {

        Undefined = 0,

        Warmup = 1 << 0,

        Start = 1 << 1,

        Countdown = 1 << 2,

        Playing = 1 << 3,

        PlayerDefeated = 1 << 4,

        GameOver = 1 << 5

    };

    /**
     * @brief Bitwise OR operator for combining match states.
     */
    [[nodiscard]] constexpr MatchState operator|(const MatchState lhs, const MatchState rhs) noexcept {
        return static_cast<MatchState>(static_cast<MatchStateType>(lhs) | static_cast<MatchStateType>(rhs));
    }

    /**
     * @brief Bitwise AND operator for masking match states.
     */
    [[nodiscard]] constexpr MatchState operator&(const MatchState lhs, const MatchState rhs) noexcept {
        return static_cast<MatchState>(static_cast<MatchStateType>(lhs) & static_cast<MatchStateType>(rhs));
    }

    /**
     * @brief Checks if a flag is set in a state mask.
     *
     * @param mask The state mask to check.
     * @param flag The flag to check for.
     * @return True if the flag is set in the mask.
     */
    [[nodiscard]] constexpr bool hasFlag(const MatchState mask, const MatchState flag) noexcept {
        return (mask & flag) == flag;
    }

}

