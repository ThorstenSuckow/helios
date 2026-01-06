/**
 * @file GameLoopEventBus.ixx
 * @brief Type-safe event bus for inter-system communication within the game loop.
 */
module;

export module helios.engine.core.messaging.event.GameLoopEventBus;

import helios.core.data.TypeIndexer;
import helios.core.buffer.TypeIndexedDoubleBuffer;


export namespace helios::engine::core::messaging::event {

    /**
     * @struct GameLoopEventBusGroup
     * @brief Tag type for the game loop event bus index space.
     *
     * @details Used as a template parameter for TypeIndexer to create a dedicated
     * index space for game loop events, separate from other typed buffer systems.
     */
    struct GameLoopEventBusGroup {};

    /**
     * @typedef GameLoopEventBus
     * @brief Double-buffered event bus for decoupled inter-system communication.
     *
     * @details GameLoopEventBus is a type alias for TypeIndexedDoubleBuffer configured
     * with a dedicated index space for game loop events. It enables systems to publish
     * events during their update phase, which become available for consumption after
     * the buffer swap at frame boundaries.
     *
     * Events are pushed during system updates and read in subsequent phases or frames,
     * enabling temporal decoupling between event producers and consumers.
     *
     * Example usage:
     * ```cpp
     * // Define an event type
     * struct CollisionEvent {
     *     EntityId a;
     *     EntityId b;
     *     vec3f contactPoint;
     * };
     *
     * // In collision detection system
     * eventBus.push<CollisionEvent>(entityA, entityB, contact);
     *
     * // At frame boundary (in game loop)
     * eventBus.swapBuffers();
     *
     * // In damage system (reads events from previous frame)
     * for (const auto& evt : eventBus.read<CollisionEvent>()) {
     *     applyDamage(evt.a, evt.b);
     * }
     * ```
     *
     * @see helios::core::buffer::TypeIndexedDoubleBuffer
     */
    using GameLoopEventBus = helios::core::buffer::TypeIndexedDoubleBuffer<
        helios::core::data::TypeIndexer<GameLoopEventBusGroup>
    >;

    /**
     * @typedef GameLoopEventSink
     * @brief Write-only handle for pushing events to the GameLoopEventBus.
     *
     * @details GameLoopEventSink provides a focused interface for event producers
     * that only need to publish events. It wraps the GameLoopEventBus and exposes
     * only the push() operation, preventing accidental buffer manipulation.
     *
     * Use this type for dependency injection when a system should be able to emit
     * events but should not have access to swap or clear operations.
     *
     * Example usage:
     * ```cpp
     * class CollisionSystem {
     * public:
     *     void update(GameLoopEventSink& sink) {
     *         // Can only push, cannot swap or read
     *         sink.push<CollisionEvent>(a, b, contact);
     *     }
     * };
     * ```
     *
     * @see GameLoopEventBus::WriteSink
     */
    using GameLoopEventSink = GameLoopEventBus::WriteSink;

}