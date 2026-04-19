/**
 * @file GameState.ixx
 * @brief Game state enumeration and bitmask operations.
 */
module;

#include <cstdint>
#include <cstddef>

export module helios.gameplay.gamestate.types.GameState;



export namespace helios::gameplay::gamestate::types {

    /**
     * @brief Underlying type for GameState bitmask values.
     */
    using GameStateType = uint16_t;

    constexpr size_t GameStateTypeSize = 16;

    /**
     * @brief Enumeration of possible game states.
     *
     * States can be combined using bitwise operators for masking.
     */
    enum class GameState : GameStateType {

        Undefined = 0,

        Booting = 1 << 0,

        Booted = 1 << 1,

        Warmup = 1 << 2,

        Title = 1 << 3,

        MatchReady = 1 << 4,

        Running = 1 << 5,

        Paused = 1 << 6,

        Shutdown = 1 << 7,


        /**
         * @brief Bitmask representing all base/ single-bit state flags.
         */
        Any = Booting | Booted | Running | MatchReady | Title | Paused | Shutdown | Warmup,

        /**
         * @brief Bitmask representing any running state, i.e. any state except Booting, Booted, Warmup and Shutdown.
         */
        Live = Any & ~(Booting | Booted | Warmup | Shutdown),


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
     * @brief Bitwise NOT operator for inverting game state bits.
     */
    [[nodiscard]] constexpr GameState operator~(const GameState lhs) noexcept {
        return static_cast<GameState>((~static_cast<GameStateType>(lhs)) & static_cast<GameStateType>(GameState::Any));
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

