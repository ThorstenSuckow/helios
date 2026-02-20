/**
 * @file PassCommitListener.ixx
 * @brief Interface for listeners notified when a pass commits its state.
 */
module;


export module helios.engine.runtime.gameloop.PassCommitListener;

import helios.engine.runtime.gameloop.CommitPoint;

import helios.engine.runtime.world.UpdateContext;

export namespace helios::engine::runtime::gameloop {

    /**
     * @brief Interface for receiving notifications when a pass reaches its commit point.
     *
     * @details
     * The PassCommitListener provides a callback mechanism for responding to pass-level
     * synchronization events within the game loop. When a Pass has a configured CommitPoint
     * and finishes execution, registered listeners are notified via `onPassCommit()`.
     *
     * This interface is primarily implemented by the GameLoop class to handle:
     * - **Event buffer swapping:** Making pass-level events readable to subsequent passes.
     * - **Command flushing:** Executing deferred commands from the CommandBuffer.
     * - **Manager flushing:** Processing queued requests (e.g., spawn/despawn operations).
     *
     * The CommitPoint flags determine which synchronization actions are performed:
     *
     * | Flag | Action |
     * |------|--------|
     * | `PassEvents` | Swaps pass event bus buffers |
     * | `FlushCommands` | Executes pending commands |
     * | `FlushManagers` | Processes manager requests |
     * | `Structural` | Combines all three flags |
     *
     * @par Integration with GameLoop
     *
     * The GameLoop implements this interface and registers itself with each Phase.
     * When a Pass with a commit point completes, the Phase calls `onPassCommit()`,
     * allowing the GameLoop to perform the appropriate synchronization.
     *
     * ```cpp
     * // In GameLoop::onPassCommit():
     * if ((commitPoint & CommitPoint::PassEvents) == CommitPoint::PassEvents) {
     *     passEventBus_.swapBuffers();
     * }
     * if ((commitPoint & CommitPoint::FlushCommands) == CommitPoint::FlushCommands) {
     *     commandBuffer_.flush(updateContext.gameWorld());
     * }
     * if ((commitPoint & CommitPoint::FlushManagers) == CommitPoint::FlushManagers) {
     *     updateContext.gameWorld().flushManagers(updateContext);
     * }
     * ```
     *
     * @see GameLoop
     * @see Phase
     * @see Pass
     * @see CommitPoint
     */
    class PassCommitListener {


    public:

        /**
         * @brief Virtual destructor for proper polymorphic cleanup.
         */
        virtual ~PassCommitListener() = default;

        /**
         * @brief Called when a pass reaches its commit point.
         *
         * @details
         * Implementations should check the CommitPoint flags and perform the
         * appropriate synchronization actions. The order of operations should be:
         *
         * 1. **PassEvents** - Swap event buffers first so subsequent actions can emit events.
         * 2. **FlushCommands** - Execute commands before managers to generate spawn requests.
         * 3. **FlushManagers** - Process requests after commands have been executed.
         *
         * @param commitPoint The flags specifying which synchronization actions to perform.
         * @param updateContext The current frame's update context.
         *
         * @note This method is marked `noexcept` as it is called during the game loop
         *       hot path and should not throw exceptions.
         *
         * @see CommitPoint
         * @see Pass::addCommitPoint()
         */
        virtual void onPassCommit(
            CommitPoint commitPoint,
            helios::engine::runtime::world::UpdateContext& updateContext) noexcept = 0;
    };


}