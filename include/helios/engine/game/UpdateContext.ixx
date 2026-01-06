/**
 * @file UpdateContext.ixx
 * @brief Context struct passed to components during per-frame updates.
 */
module;

#include <cassert>
#include <stdexcept>

export module helios.engine.game.UpdateContext;

import helios.input.InputSnapshot;
import helios.engine.core.messaging.event.GameLoopEventBus;

export namespace helios::engine::core::messaging::command {
    class CommandBuffer;
}


export namespace helios::engine::game {


    class GameWorld;

    /**
     * @brief Context passed to Updatable components during per-frame updates.
     *
     * @details Provides all necessary state for components to perform their update logic,
     * including frame timing, input state, and access to the command system.
     * All pointers are non-owning; the caller is responsible for lifetime management.
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
        helios::engine::game::GameWorld& gameWorld_;

        /**
         * @brief Sink for pushing game loop events during update.
         *
         * Used by systems and components to publish events (e.g., collision,
         * spawn requests) that will be processed in a later phase (N+1) of the game loop.
         */
        helios::engine::core::messaging::event::GameLoopEventSink eventSink_;

    public:

        /**
         * @brief Constructs an UpdateContext with required dependencies.
         *
         * @param commandBuffer Non-owning pointer to the command buffer. Must not be nullptr.
         * @param gameWorld Non-owning pointer to the game world. Must not be nullptr.
         * @param eventSink Sink for pushing game loop events. Used to publish events
         *                  during update phases for later processing.
         *
         * @throws std::invalid_argument if either commandBuffer or gameWorld are nullptr.
         */
        UpdateContext(
            helios::engine::core::messaging::command::CommandBuffer& commandBuffer,
            helios::engine::game::GameWorld& gameWorld,
            const helios::engine::core::messaging::event::GameLoopEventSink& eventSink
        ) : commandBuffer_(commandBuffer), gameWorld_(gameWorld), eventSink_(eventSink) {}

        /**
         * @brief Returns the time elapsed since the last frame, in seconds.
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
        [[nodiscard]] helios::engine::game::GameWorld& gameWorld() const noexcept {
            return gameWorld_;
        }

        /**
         * @brief Pushes an event to the game loop event bus.
         *
         * @details This method allows systems and components to publish events during
         * their update phase. Events are buffered and processed in a subsequent phase
         * (N+1) of the game loop, ensuring decoupled communication between systems.
         *
         * @tparam E The event type to push.
         * @tparam Args Constructor argument types for the event.
         *
         * @param args Arguments forwarded to the event constructor.
         *
         * Example usage:
         * ```cpp
         * updateContext.pushEvent<CollisionEvent>(entityA, entityB, contactPoint);
         * ```
         */
        template<typename E, typename... Args>
        void pushEvent(Args&&... args) {
            eventSink_.template push<E>(std::forward<Args>(args)...);
        }
    };
}