/**
 * @file GameState.ixx
 * @brief Game state enumeration and bitmask operations.
 */
module;

#include <cstdint>

export module helios.engine.mechanics.gamestate.types.GameState;



export namespace helios::engine::mechanics::gamestate::types {

    /**
     * @brief Underlying type for GameState bitmask values.
     */
    using GameStateType = uint16_t;

    /**
     * @brief Enumeration of possible game states.
     *
     * States can be combined using bitwise operators for masking.
     */
    enum class GameState : GameStateType {

        Undefined = 0,

        Loading = 1 << 1,

        Title = 1 << 2,

        Menu = 1 << 3,

        Paused = 1 << 4,

        Running = 1 << 5,

        /**
         * @brief Bitmask representing all defined states.
         */
        Any = Loading | Title | Menu | Paused | Running

    };

    /**
     * @brief Bitwise OR operator for combining game states.
     */
    [[nodiscard]] constexpr GameState operator|(const GameState lhs, const GameState rhs) noexcept {
        return static_cast<GameState>(static_cast<GameStateType>(lhs) | static_cast<GameStateType>(rhs));
    }

    /**
     * @brief Bitwise AND operator for masking game states.
     */
    [[nodiscard]] constexpr GameState operator&(const GameState lhs, const GameState rhs) noexcept {
        return static_cast<GameState>(static_cast<GameStateType>(lhs) & static_cast<GameStateType>(rhs));
    }

    /**
     * @brief Checks if a flag is set in a state mask.
     *
     * @param mask The state mask to check.
     * @param flag The flag to check for.
     * @return True if the flag is set in the mask.
     */
    [[nodiscard]] constexpr bool hasFlag(const GameState mask, const GameState flag) noexcept {
        return (mask & flag) == flag;
    }

}

