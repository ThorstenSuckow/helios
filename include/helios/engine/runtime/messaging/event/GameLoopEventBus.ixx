/**
 * @file GameLoopEventBus.ixx
 * @brief Type-safe event bus for inter-system communication within the game loop.
 */
module;

export module helios.engine.runtime.messaging.event.GameLoopEventBus;

import helios.core.data.TypeIndexer;
import helios.core.buffer.TypeIndexedDoubleBuffer;


export namespace helios::engine::runtime::messaging::event {

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
     * the buffer swap.
     *
     * ## Phase/Pass Event Model
     *
     * The GameLoop uses multiple event buses to control event visibility:
     *
     * - **Pass Events:** Events written with `push()` are available to subsequent passes
     *   within the **same phase only**. They are **always cleared** at the end of the phase.
     *
     * - **Phase Events:** Events written with `phasePush()` are preserved across phase
     *   boundaries and available in subsequent phases of the same frame.
     *
     * This allows fine-grained control over event propagation:
     *
     * ```
     * ┌─────────────────────────────────────────────────────────────┐
     * │                         FRAME                               │
     * ├─────────────────────────────────────────────────────────────┤
     * │  PRE PHASE                                                  │
     * │    Pass 1: push<InputEvent>()     → available in Pass 2     │
     * │    Pass 2: read<InputEvent>()     ✓ works                   │
     * │            phasePush<SpawnReq>()  → available in MAIN       │
     * │    [Phase Commit] ← Pass events CLEARED                     │
     * │                                                             │
     * │  MAIN PHASE                                                 │
     * │    Pass 1: read<InputEvent>()     ✗ EMPTY (cleared!)        │
     * │            read<SpawnReq>()       ✓ works (phasePush)       │
     * │            push<CollisionEvent>() → available in Pass 2     │
     * │    Pass 2: read<CollisionEvent>() ✓ works                   │
     * │    [Phase Commit] ← Pass events CLEARED                     │
     * │                                                             │
     * │  POST PHASE                                                 │
     * │    Pass 1: read<CollisionEvent>() ✗ EMPTY (cleared!)        │
     * │            read<SpawnReq>()       ✓ works (phasePush)       │
     * │    [Phase Commit] ← All events CLEARED for next frame       │
     * └─────────────────────────────────────────────────────────────┘
     * ```
     *
     * ## Usage Example
     *
     * ```cpp
     * // Define an event type
     * struct CollisionEvent {
     *     Guid entityA;
     *     Guid entityB;
     *     vec3f contactPoint;
     * };
     *
     * // In collision detection system (Main Phase, Pass 1)
     * // Use push() for events only needed within this phase
     * eventBus.push<CollisionEvent>(entityA, entityB, contact);
     *
     * // In damage system (Main Phase, Pass 2) - same phase, works!
     * for (const auto& evt : eventBus.read<CollisionEvent>()) {
     *     applyDamage(evt.entityA, evt.entityB);
     * }
     *
     * // For events needed in subsequent phases, use phasePush()
     * eventBus.phasePush<SpawnRequestEvent>(poolId, position);
     * ```
     *
     * @warning Pass events (`push()`) are **always cleared** at phase boundaries.
     *          They are NOT available in subsequent phases. Use `phasePush()` for
     *          events that must survive into subsequent phases.
     *
     * @see helios::core::buffer::TypeIndexedDoubleBuffer
     * @see UpdateContext — Provides access to the event bus in systems
     * @see GameLoop — Manages event bus lifecycle and commit points
     */
    using GameLoopEventBus = helios::core::buffer::TypeIndexedDoubleBuffer<
        helios::core::data::TypeIndexer<GameLoopEventBusGroup>
    >;



}