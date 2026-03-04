/**
 * @file UpdateContext.ixx
 * @brief Per-frame context passed to systems during game loop updates.
 */
module;

#include <span>
#include <optional>

export module helios.engine.runtime.world.UpdateContext;

import helios.input.InputSnapshot;
import helios.rendering.ViewportSnapshot;

import helios.engine.runtime.world.Level;
import helios.engine.runtime.messaging.event.GameLoopEventBus;

import helios.engine.runtime.world.ResourceRegistry;

import helios.engine.ecs.EntityResolver;
import helios.engine.ecs.GameObject;
import helios.engine.ecs.EntityHandle;

import helios.engine.ecs.View;

export namespace helios::engine::runtime::messaging::command  {
    class EngineCommandBuffer;
}

using namespace helios::engine::runtime::messaging::command;

export namespace helios::engine::runtime::world {

    class Session;


    /**
     * @brief Per-frame context passed to systems during game loop updates.
     *
     * @details UpdateContext bundles all state that systems need for a single
     * frame update: timing, input, resource access, entity resolution, event
     * buses, and the active level. It is constructed by the GameLoop each
     * frame and passed to every System::update() call.
     *
     * ## Resource Access
     *
     * - `commandBuffer()` — returns the EngineCommandBuffer for submitting
     *   engine-defined commands (shortcut for `resourceRegistry().resource<EngineCommandBuffer>()`)
     * - `resourceRegistry()` — O(1) lookup for Managers, CommandBuffers,
     *   CommandHandlers, and other registered resources
     * - `session()` — cross-frame state (tracked game/match states)
     * - `level()` — current Level with arena bounds
     *
     * ## Entity Access
     *
     * - `find(handle)` — resolves an EntityHandle to a GameObject (validates
     *   the handle's version via the EntityResolver)
     * - `entityResolver()` — direct access to the callable resolver
     *
     * ## Event Propagation
     *
     * | Level | Push | Read | Scope |
     * |-------|------|------|-------|
     * | Pass  | `pushPass()` | `readPass()` | Within same phase, after commit point |
     * | Phase | `pushPhase()` | `readPhase()` | Across phases within same frame |
     * | Frame | `pushFrame()` | `readFrame()` | Across frames |
     *
     * @see GameLoop
     * @see ResourceRegistry
     * @see EngineCommandBuffer
     * @see Session
     * @see EntityResolver
     * @see InputSnapshot
     */
    class UpdateContext {

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
         * @brief Reference to the current game session for state tracking.
         */
        helios::engine::runtime::world::Session& session_;

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

        /**
         * @brief Immutable snapshot of all viewport states for this frame.
         */
        std::span<const helios::rendering::ViewportSnapshot> viewportSnapshots_;

        /**
         * @brief Reference to the ResourceRegistry for O(1) resource lookup.
         */
        helios::engine::runtime::world::ResourceRegistry& resourceRegistry_;

        /**
         * @brief Callable for resolving EntityHandles to GameObjects.
         */
        helios::engine::ecs::EntityResolver entityResolver_;

        /**
         * @brief Pointer to the active Level, or nullptr if no level is loaded.
         */
        const Level* level_;

    public:


        /**
         * @brief Constructs an UpdateContext with all per-frame dependencies.
         *
         * @param resourceRegistry Reference to the ResourceRegistry for resource lookup.
         * @param entityResolver Callable for resolving EntityHandles to GameObjects.
         * @param session Reference to the current game session.
         * @param deltaTime Time since last frame in seconds.
         * @param totalTime Accumulated time since the first frame in seconds.
         * @param phaseEventBus Reference to the phase-level event bus.
         * @param passEventBus Reference to the pass-level event bus.
         * @param frameEventBus Reference to the frame-level event bus.
         * @param inputSnapshot Immutable input state for this frame.
         * @param viewportSnapshots Immutable snapshot of viewport states.
         * @param level Pointer to the active Level, or nullptr.
         */
        UpdateContext(
            helios::engine::runtime::world::ResourceRegistry& resourceRegistry,
            helios::engine::ecs::EntityResolver entityResolver,
            helios::engine::runtime::world::Session& session,
            const float deltaTime,
            const float totalTime,
            helios::engine::runtime::messaging::event::GameLoopEventBus& phaseEventBus,
            helios::engine::runtime::messaging::event::GameLoopEventBus& passEventBus,
            helios::engine::runtime::messaging::event::GameLoopEventBus& frameEventBus,
            const helios::input::InputSnapshot& inputSnapshot,
            std::span<const helios::rendering::ViewportSnapshot> viewportSnapshots,
            const Level* level
        ) :
        resourceRegistry_(resourceRegistry),
        entityResolver_(entityResolver),
        session_(session),
        deltaTime_(deltaTime),
        totalTime_(totalTime),
        phaseEventSink_(phaseEventBus.writeSink()),
        phaseEventSource_(phaseEventBus.readSource()),
        passEventSink_(passEventBus.writeSink()),
        passEventSource_(passEventBus.readSource()),
        frameEventSink_(frameEventBus.writeSink()),
        frameEventSource_(frameEventBus.readSource()),
        inputSnapshot_(inputSnapshot),
        viewportSnapshots_(viewportSnapshots),
        level_(level)
        {

        }

        /**
         * @brief Returns the viewport snapshots for this frame.
         *
         * @return A span of const ViewportSnapshot objects.
         */
        [[nodiscard]] std::span<const helios::rendering::ViewportSnapshot> viewportSnapshots() const noexcept {
            return viewportSnapshots_;
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
         * @brief Returns the ResourceRegistry for resource lookup.
         *
         * @return Reference to the ResourceRegistry.
         */
        [[nodiscard]] helios::engine::runtime::world::ResourceRegistry& resourceRegistry() noexcept {
            return resourceRegistry_;
        }

        /**
         * @copydoc resourceRegistry()
         */
        [[nodiscard]] const helios::engine::runtime::world::ResourceRegistry& resourceRegistry() const noexcept {
            return resourceRegistry_;
        }

        /**
         * @brief Resolves an EntityHandle to a GameObject.
         *
         * @details Validates the handle via the EntityResolver and returns
         * a lightweight GameObject wrapper if the entity is still alive.
         *
         * @param handle The entity handle to resolve.
         *
         * @return A GameObject if the handle is valid, std::nullopt otherwise.
         */
        [[nodiscard]] std::optional<helios::engine::ecs::GameObject> find(helios::engine::ecs::EntityHandle handle) const noexcept {
            return entityResolver_(handle);
        }


        /**
         * @brief Returns the EntityResolver for direct handle validation.
         *
         * @return Reference to the EntityResolver.
         */
        [[nodiscard]] helios::engine::ecs::EntityResolver& entityResolver() noexcept {
            return entityResolver_;
        }

        /**
         * @brief Returns the active Level.
         *
         * @return Pointer to the Level, or nullptr if no level is loaded.
         */
        [[nodiscard]] const Level* level() noexcept {
            return level_;
        }

        /**
         * @brief Returns the EngineCommandBuffer for submitting commands.
         *
         * @details Shortcut for `resourceRegistry().resource<EngineCommandBuffer>()`.
         * Only accepts engine-defined command types. For custom commands, look up
         * the appropriate handler or buffer via `resourceRegistry()` directly.
         *
         * @return Reference to the EngineCommandBuffer.
         */
        [[nodiscard]] EngineCommandBuffer& commandBuffer () const noexcept {
            return resourceRegistry_.resource<EngineCommandBuffer>();
        }

        /**
         * @brief Returns the session for game/match state access.
         *
         * @return Ref to the Session used with this UpdateContext.
         */
        [[nodiscard]] helios::engine::runtime::world::Session& session() const noexcept {
            return session_;
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


        /**
         * @brief Creates a View for querying entities with the given component types.
         *
         * @details Convenience shortcut that avoids accessing the GameWorld
         * directly. Delegates to the EntityManager owned by the
         * EntityResolver.
         *
         * @tparam Components The component types to query.
         *
         * @return A View over all entities possessing every requested component.
         *
         * @see helios::engine::ecs::View
         */
        template <typename... Components>
        [[nodiscard]] auto view() {
            return helios::engine::ecs::View<Components...>(entityResolver_.em);
        }

        /**
         * @copydoc view()
         */
        template <typename... Components>
        [[nodiscard]] auto view() const {
            return helios::engine::ecs::View<const Components...>(entityResolver_.em);
        }

    };
}