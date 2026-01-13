/**
 * @file UpdateContext.ixx
 * @brief Context struct passed to components during per-frame updates.
 */
module;

#include <cassert>
#include <span>
#include <stdexcept>

export module helios.engine.runtime.world.UpdateContext;

import helios.input.InputSnapshot;
import helios.engine.core.messaging.event.GameLoopEventBus;

export namespace helios::engine::core::messaging::command {
    class CommandBuffer;
}


export namespace helios::engine::runtime::world {



    class GameWorld;

    /**
     * @brief Context passed to systems and components during per-frame updates.
     *
     * Provides all necessary state for systems to perform their update logic,
     * including frame timing, input state, command buffer access, and event propagation.
     *
     * The UpdateContext serves as the central communication hub within the game loop,
     * offering two levels of event propagation:
     *
     * **Pass-level events** are pushed via pushPass() and become readable in subsequent
     * passes within the same phase after a commit point is reached.
     *
     * **Phase-level events** are pushed via pushPhase() and become readable in subsequent
     * phases after the current phase commits.
     *
     * @see GameLoop
     * @see Pass
     */
    struct UpdateContext {

    private:
        /**
         * @brief Time elapsed since the last frame, in seconds.
         */
        float deltaTime_ = 0.0f;

        /**
         * @brief Immutable snapshot of input state for the current frame.
         */
        const helios::input::InputSnapshot* inputSnapshot_ = nullptr;

        /**
         * @brief Buffer for queueing commands to be executed at end of frame.
         */
        helios::engine::core::messaging::command::CommandBuffer& commandBuffer_;

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
        helios::engine::core::messaging::event::GameLoopEventBus::WriteSink phaseEventSink_;

        /**
         * @brief Source for reading phase-level events from the previous phase.
         */
        const helios::engine::core::messaging::event::GameLoopEventBus::ReadSource phaseEventSource_;

        /**
         * @brief Sink for pushing pass-level events during update.
         *
         * Used by systems and components to publish events that will be processed
         * in subsequent passes within the same phase, after a commit point.
         */
        helios::engine::core::messaging::event::GameLoopEventBus::WriteSink passEventSink_;

        /**
         * @brief Source for reading pass-level events from previous passes.
         */
        const helios::engine::core::messaging::event::GameLoopEventBus::ReadSource passEventSource_;

    public:


        /**
         * @brief Constructs an UpdateContext with required dependencies.
         *
         * @param commandBuffer Reference to the command buffer for queueing commands.
         * @param gameWorld Reference to the game world for entity lookups.
         * @param phaseEventBus Reference to the phase-level event bus for cross-phase communication.
         * @param passEventBus Reference to the pass-level event bus for cross-pass communication.
         */
        UpdateContext(
            helios::engine::core::messaging::command::CommandBuffer& commandBuffer,
            helios::engine::runtime::world::GameWorld& gameWorld,
            helios::engine::core::messaging::event::GameLoopEventBus& phaseEventBus,
            helios::engine::core::messaging::event::GameLoopEventBus& passEventBus
        ) : commandBuffer_(commandBuffer), gameWorld_(gameWorld),
        phaseEventSink_(phaseEventBus.writeSink()),
        phaseEventSource_(phaseEventBus.readSource()),
        passEventSink_(passEventBus.writeSink()),
        passEventSource_(passEventBus.readSource())
        {}

        /**
         * @brief Returns the time elapsed since the last frame, in seconds.
         *
         * @return Delta time in seconds.
         */
        [[nodiscard]] float deltaTime() const noexcept {
            return deltaTime_;
        }

        /**
         * @brief Sets the time elapsed since the last frame.
         *
         * @param dt Delta time in seconds. Must be non-negative.
         *
         * @return A reference to this UpdateContext instance.
         */
        UpdateContext& setDeltaTime(float dt) noexcept {
            deltaTime_ = dt;

            return *this;
        }

        /**
         * @brief Returns the immutable input snapshot for this frame.
         *
         * @return Const ref to the current InputSnapshot.
         */
        [[nodiscard]] const helios::input::InputSnapshot& inputSnapshot() const noexcept {
            assert(inputSnapshot_ && "Unexpected nullptr for InputSnapshot");
            return *inputSnapshot_;
        }

        /**
         * @brief Sets the input snapshot for this frame.
         *
         * @param snapshot Const ref to the input snapshot.
         *
         * @return A reference to this UpdateContext instance.
         */
        UpdateContext& setInputSnapshot(const helios::input::InputSnapshot& snapshot) noexcept {
            inputSnapshot_ = &snapshot;

            return *this;
        }

        /**
         * @brief Returns the command buffer for queueing commands.
         *
         * @return Ref to the CommandBuffer used with this UpdateContext.
         */
        [[nodiscard]] helios::engine::core::messaging::command::CommandBuffer& commandBuffer() const noexcept {
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
    };
}