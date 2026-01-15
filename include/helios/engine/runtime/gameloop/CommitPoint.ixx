/**
 * @file CommitPoint.ixx
 * @brief Defines commit points for synchronization within the game loop.
 */
module;

#include <cstdint>
#include <utility>

export module helios.engine.runtime.gameloop.CommitPoint;


export namespace helios::engine::runtime::gameloop {

    /**
     * @brief Flags defining synchronization actions at the end of a pass.
     *
     * @details A CommitPoint specifies what operations should be performed when a pass
     * completes. Multiple flags can be combined using bitwise OR to trigger several
     * actions at once.
     *
     * Commit points are essential for the deterministic ordering of the game loop:
     * - Commands are queued during passes and executed at commit points
     * - Pass-level events become readable only after a commit
     * - Managers process their queued requests at commit points
     *
     * Example usage:
     * ```cpp
     * pass.addCommitPoint(CommitPoint::PassEvents | CommitPoint::FlushCommands);
     * ```
     *
     * @see Pass::addCommitPoint()
     * @see GameLoop::passCommit()
     */
    enum class CommitPoint : uint8_t {

        /**
         * @brief No commit action.
         */
        None = 0,

        /**
         * @brief Commits pass-level events.
         *
         * Events pushed via `UpdateContext::pushPass()` become readable in subsequent
         * passes through `UpdateContext::readPass()`. Pass events are discarded at the
         * end of the phase.
         */
        PassEvents = 1 << 0,

        /**
         * @brief Flushes the command buffer.
         *
         * Pending commands are dispatched to their registered handlers and executed.
         */
        FlushCommands  = 1 << 1,

        /**
         * @brief Flushes all registered managers.
         *
         * Managers process their queued requests (e.g., ProjectileManager spawns
         * projectiles from its request queue).
         */
        FlushManagers   = 1 << 2,

        /**
         * @brief Full structural commit.
         *
         * Combines PassEvents, FlushManagers, and FlushCommands for a complete
         * synchronization point.
         */
        Structural = PassEvents | FlushCommands | FlushManagers
    };

    /**
     * @brief Bitwise AND operator for CommitPoint flags.
     *
     * @param a First operand.
     * @param b Second operand.
     *
     * @return The combined flags.
     */
    CommitPoint constexpr operator &(CommitPoint a, CommitPoint b) noexcept {
        return static_cast<CommitPoint>(std::to_underlying(a) & std::to_underlying(b));
    }

    /**
     * @brief Bitwise OR operator for CommitPoint flags.
     *
     * @param a First operand.
     * @param b Second operand.
     *
     * @return The combined flags.
     */
    CommitPoint constexpr operator |(CommitPoint a, CommitPoint b) noexcept {
        return static_cast<CommitPoint>(std::to_underlying(a) | std::to_underlying(b));
    }


}
