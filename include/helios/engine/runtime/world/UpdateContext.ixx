/**
 * @file UpdateContext.ixx
 * @brief Context struct passed to components during per-frame updates.
 */
module;

#include <span>

export module helios.engine.runtime.world.UpdateContext;

import helios.input.InputSnapshot;
import helios.engine.runtime.messaging.event.GameLoopEventBus;

export namespace helios::engine::runtime::messaging::command {
    class CommandBuffer;
}


export namespace helios::engine::runtime::world {



    class GameWorld;

    /**
     * @brief Context passed to systems and components during per-frame updates.
     *
     * @details Provides all necessary state for systems to perform their update logic,
     * including frame timing, input state, command buffer access, and event propagation.
     *
     * The UpdateContext serves as the central communication hub within the game loop,
     * offering three levels of event propagation:
     *
     * ## Event Levels
     *
     * | Level | Push Method | Read Method | Scope |
     * |-------|-------------|-------------|-------|
     * | Pass  | `pushPass()` | `readPass()` | Within same phase, after commit point |
     * | Phase | `pushPhase()` | `readPhase()` | Across phases within same frame |
     * | Frame | `pushFrame()` | `readFrame()` | Across frames |
     *
     * **Pass-level events** are pushed via `pushPass()` and become readable in subsequent
     * passes within the same phase after a commit point is reached. Cleared at phase end.
     *
     * **Phase-level events** are pushed via `pushPhase()` and become readable in subsequent
     * phases after the current phase commits. Cleared at frame end.
     *
     * **Frame-level events** are pushed via `pushFrame()` and become readable in the next
     * frame. Useful for cross-frame communication (e.g., collision responses, spawn confirmations).
     *
     * @see GameLoop
     * @see Pass
     * @see CommandBuffer
     * @see InputSnapshot
     */
    struct UpdateContext {

    private:
        /**
         * @brief Time elapsed since the last frame, in seconds.
         */
        float deltaTime_ = 0.0f;

        /**
        * @brief Time elapsed since the first frame, in seconds.
         */
        float totalTime_ = 0.0f;

        /**
         * @brief Immutable snapshot of input state for the current frame.
         */
        const helios::input::InputSnapshot& inputSnapshot_;

        /**
         * @brief Buffer for queueing commands to be executed at end of frame.
         */
        helios::engine::runtime::messaging::command::CommandBuffer& commandBuffer_;

        /**
         * @brief Reference to the game world for entity lookups.
         */
        helios::engine::runtime::world::GameWorld& gameWorld_;

        /**
         * @brief Sink for pushing phase-level events during update.
         *
         * Used by systems and components to publish events (e.g., collision,
         * spawn requests) that will be processed in the next phase of the game loop.
         */
        helios::engine::runtime::messaging::event::GameLoopEventBus::WriteSink phaseEventSink_;

        /**
         * @brief Source for reading phase-level events from the previous phase.
         */
        const helios::engine::runtime::messaging::event::GameLoopEventBus::ReadSource phaseEventSource_;

        /**
         * @brief Sink for pushing pass-level events during update.
         *
         * Used by systems and components to publish events that will be processed
         * in subsequent passes within the same phase, after a commit point.
         */
        helios::engine::runtime::messaging::event::GameLoopEventBus::WriteSink passEventSink_;

        /**
         * @brief Source for reading pass-level events from previous passes.
         */
        const helios::engine::runtime::messaging::event::GameLoopEventBus::ReadSource passEventSource_;

        /**
         * @brief Sink for pushing frame-level events during update.
         *
         * Used by systems and components to publish events that will be processed
         * in the next frame. Frame-level events persist across all phases and are
         * swapped at the end of the Post phase.
         */
        helios::engine::runtime::messaging::event::GameLoopEventBus::WriteSink frameEventSink_;

        /**
         * @brief Source for reading frame-level events from the previous frame.
         */
        const helios::engine::runtime::messaging::event::GameLoopEventBus::ReadSource frameEventSource_;

    public:


        /**
         * @brief Constructs an UpdateContext with required dependencies.
         *
         * @param commandBuffer Reference to the command buffer for queueing commands.
         * @param gameWorld Reference to the game world for entity lookups.
         * @param deltaTime Time since last frame / update in seconds
         * @param phaseEventBus Reference to the phase-level event bus for cross-phase communication.
         * @param passEventBus Reference to the pass-level event bus for cross-pass communication.
         * @param inputSnapshot The input snapshot for this frame.
         */
        UpdateContext(
            helios::engine::runtime::messaging::command::CommandBuffer& commandBuffer,
            helios::engine::runtime::world::GameWorld& gameWorld,
            const float deltaTime,
            helios::engine::runtime::messaging::event::GameLoopEventBus& phaseEventBus,
            helios::engine::runtime::messaging::event::GameLoopEventBus& passEventBus,
            helios::engine::runtime::messaging::event::GameLoopEventBus& frameEventBus,
            const helios::input::InputSnapshot& inputSnapshot
        ) : commandBuffer_(commandBuffer), gameWorld_(gameWorld),
        deltaTime_(deltaTime),
        totalTime_(totalTime_ + deltaTime),
        phaseEventSink_(phaseEventBus.writeSink()),
        phaseEventSource_(phaseEventBus.readSource()),
        passEventSink_(passEventBus.writeSink()),
        passEventSource_(passEventBus.readSource()),
        frameEventSink_(frameEventBus.writeSink()),
        frameEventSource_(frameEventBus.readSource()),
        inputSnapshot_(inputSnapshot) {

        }

        /**
         * @brief Returns the time elapsed since the last frame, in seconds.
         *
         * @return Delta time in seconds.
         */
        [[nodiscard]] float deltaTime() const noexcept {
            return deltaTime_;
        }

        /**
         * @brief Returns the time elapsed since the first frame, in seconds.
         *
         * @return Total time in seconds.
         */
        [[nodiscard]] float totalTime() const noexcept {
            return totalTime_;
        }

        /**
         * @brief Returns the immutable input snapshot for this frame.
         *
         * @return Const ref to the current InputSnapshot.
         */
        [[nodiscard]] const helios::input::InputSnapshot& inputSnapshot() const noexcept {
            return inputSnapshot_;
        }

        /**
         * @brief Returns the command buffer for queueing commands.
         *
         * @return Ref to the CommandBuffer used with this UpdateContext.
         */
        [[nodiscard]] helios::engine::runtime::messaging::command::CommandBuffer& commandBuffer() const noexcept {
            return commandBuffer_;
        }

        /**
         * @brief Returns the game world for entity lookups.
         *
         * @return Ref to the GameWorld used with this UpdateContext.
         */
        [[nodiscard]] helios::engine::runtime::world::GameWorld& gameWorld() const noexcept {
            return gameWorld_;
        }

        /**
         * @brief Pushes an event to the pass-level event bus.
         *
         * Events pushed here become readable in subsequent passes within the same
         * phase, after a commit point is reached.
         *
         * @tparam E The event type to push.
         * @tparam Args Constructor argument types for the event.
         *
         * @param args Arguments forwarded to the event constructor.
         *
         * @see readPass()
         * @see Pass::addCommitPoint()
         */
        template<typename E, typename... Args>
        void pushPass(Args&&... args) {
            passEventSink_.template push<E>(std::forward<Args>(args)...);
        }

        /**
         * @brief Pushes an event to the phase-level event bus.
         *
         * Events pushed here become readable in subsequent phases,
         * after the current phase commits.
         *
         * @tparam E The event type to push.
         * @tparam Args Constructor argument types for the event.
         *
         * @param args Arguments forwarded to the event constructor.
         *
         * @see readPhase()
         * @see GameLoop
         */
        template<typename E, typename... Args>
        void pushPhase(Args&&... args) {
            phaseEventSink_.template push<E>(std::forward<Args>(args)...);
        }

        /**
         * @brief Reads events from the phase-level event bus.
         *
         * Returns events that were pushed during the previous phase via
         * `pushPhase()`. The phase event bus is swapped at phase boundaries,
         * configured in GameLoop::phaseCommit().
         *
         * @tparam E The event type to read.
         *
         * @return A span of const events of type E.
         *
         * @see pushPhase()
         * @see GameLoop
         */
        template<typename E>
        std::span<const E> readPhase() {
            return phaseEventSource_.template read<E>();
        }

        /**
         * @brief Reads events from the pass-level event bus.
         *
         * Returns events that were pushed during previous passes within the
         * current phase via `pushPass()`. The pass event bus is swapped at
         * commit points, configured via Pass::addCommitPoint().
         *
         * @tparam E The event type to read.
         *
         * @return A span of const events of type E.
         *
         * @see pushPass()
         * @see Pass::addCommitPoint()
         */
        template<typename E>
        std::span<const E> readPass() {
            return passEventSource_.template read<E>();
        }

        /**
         * @brief Reads events from the frame-level event bus.
         *
         * @details Returns events that were pushed during the previous frame via
         * `pushFrame()`. The frame event bus is swapped at the end of the Post
         * phase, making events readable in the subsequent frame.
         *
         * Frame-level events are useful for cross-frame communication, such as:
         * - Collision events that trigger effects in the next frame
         * - Spawn confirmations for UI updates
         * - Audio/VFX triggers
         *
         * @tparam E The event type to read.
         *
         * @return A span of const events of type E.
         *
         * @see pushFrame()
         * @see GameLoop
         */
        template<typename E>
        std::span<const E> readFrame() {
            return frameEventSource_.template read<E>();
        }

        /**
         * @brief Pushes an event to the frame-level event bus.
         *
         * @details Events pushed here become readable in the next frame via
         * `readFrame()`. The frame event bus is swapped at the end of the Post
         * phase in GameLoop.
         *
         * Use frame-level events for cross-frame communication where events
         * should persist beyond the current phase.
         *
         * @tparam E The event type to push.
         * @tparam Args Constructor argument types for the event.
         *
         * @param args Arguments forwarded to the event constructor.
         *
         * @see readFrame()
         * @see GameLoop
         */
        template<typename E, typename... Args>
        void pushFrame(Args&&... args) {
            frameEventSink_.template push<E>(std::forward<Args>(args)...);
        }
    };
}