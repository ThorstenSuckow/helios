# Game Loop Architecture

helios distinguishes between **Commands** (world-mutating operations) and **Events** (signals/facts).
Commands exist to mutate the world state _deterministically_. Events exist to decouple systems: they either express a request/intent (e.g. SpawnRequest) or a fact (e.g. `SolidCollisionEvent`, `TriggerCollisionEvent`, `SpawnedEvent`).

## Commands and CommandBuffer

Participating systems can write Commands into the CommandBuffer in frame N.
At the beginning of each frame N+1, the CommandBuffer is flushed, which commits the Commands - i.e. their `execute()` method is invoked. This method contains the logic that mutates the world state (e.g. spawning, despawning, health changes, component changes). Commands are "bare metal" and therefore the lowest level in the game-loop layer, i.e. no further preparation of a Command is required. The system should therefore also be able to commit Commands coming directly from a developer console into the GameWorld (optionally delegating them to their respective managers - see below).

## Events and double-buffered EventBus

In addition, systems can emit Events in frame N, e.g. **request events** - events that intend to mutate the world state - or plain signals such as `SolidCollisionEvent`, from which world-mutating Commands (despawn) can be derived.
The EventBus is double-buffered (`helios.core.data.TypeIndexedDoubleBuffer`): events are written into the write buffer in frame N and become visible in the read buffer only after a buffer swap (`swapBuffers()`) at the beginning of frame N+1.

At the beginning of frame N+1, events are dispatched from the read buffer. As mentioned above, request events can be translated into Commands and written into the CommandBuffer.

This yields the following order at the beginning of frame N+1:

```cpp
EventBus.swapBuffers()

EventBus.dispatch() // Events from which Commands may be generated

CommandBuffer.flush() // Commit - mutates the world state,
                      // optional manager-based command processing
```

### Optional manager-based command processing

Instead of having each Command fully implement the mutation logic in `execute()`, the CommandBuffer can additionally hold registered managers. In this model, `execute()` primarily performs a registering/planning step, and the managers then process their work as a coordinated batch.

Example:

```cpp
CommandBuffer::flush() {
    for (auto& cmd : commands)
        cmd->execute();   // in execute e.g.:
                          // spawnManager->enqueue(position, enemyType);

    for (auto& mgr : managers)
        mgr->process();   // the spawnManager processes the spawn list
}
```

This model allows Commands to be bundled, sorted, and processed deterministically (e.g. handle all spawns first, then all despawns).

## Immediate Events (single-buffered)

For time-critical feedback that does not mutate the world state (particle effects, audio feedback), there is a separate **ImmediateBus** (single-buffered).
Immediate events should be processed within the same frame without additional double-buffer latency. A suitable dispatch point is after `CommandBuffer.flush()`, so that feedback can observe the world that was committed at the beginning of the frame in a consistent state. This mitigates latency (e.g. ~16ms at 60fps) and allows feedback to be triggered in the same frame as a specific event, which improves the overall user experience ("game feel").

## Frame Order Summary

Overall, the frame-begin order becomes:

```cpp
EventBus.swapBuffers() // input
EventBus.dispatch()

CommandBuffer.flush() // mutation

ImmediateBus.dispatch() // signals

// gameplay systems updating physics
Move2DSystem.update();
// ...
```
