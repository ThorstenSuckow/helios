# Event System

This document describes the event system in helios, including the double-buffered event buses, phase/pass event propagation, and integration with the game loop.

## Overview

The Event System provides a mechanism for **decoupled communication** between systems. Unlike Commands that mutate world state directly, Events are signals that inform other systems about something that happened or is requested. Events enable loose coupling: the producer doesn't need to know who consumes the event.

```
┌────────────────────────────────────────────────────────────────────────┐
│                              FRAME N                                   │
├────────────────────────────────────────────────────────────────────────┤
│                                                                        │
│  ┌──────────────┐    pushPhase()    ┌──────────────────────────────┐   │
│  │   System A   │ ─────────────────>│     Phase Event Bus          │   │
│  │              │                   │  (write buffer)              │   │
│  └──────────────┘                   └──────────────────────────────┘   │
│                                                                        │
│  ┌──────────────┐    pushPass()     ┌──────────────────────────────┐   │
│  │   System B   │ ─────────────────>│     Pass Event Bus           │   │
│  │              │                   │  (write buffer)              │   │
│  └──────────────┘                   └──────────────────────────────┘   │
│                                                                        │
└────────────────────────────────────────────────────────────────────────┘
                              │
                              │ Phase/Pass Commit
                              ▼
┌────────────────────────────────────────────────────────────────────────┐
│                           FRAME N+1 / NEXT PASS                        │
├────────────────────────────────────────────────────────────────────────┤
│                                                                        │
│  ┌──────────────────────────────┐    readPhase()    ┌──────────────┐   │
│  │     Phase Event Bus          │ ─────────────────>│   System C   │   │
│  │  (read buffer)               │                   │              │   │
│  └──────────────────────────────┘                   └──────────────┘   │
│                                                                        │
│  ┌──────────────────────────────┐    readPass()     ┌──────────────┐   │
│  │     Pass Event Bus           │ ─────────────────>│   System D   │   │
│  │  (read buffer)               │                   │              │   │
│  └──────────────────────────────┘                   └──────────────┘   │
│                                                                        │
└────────────────────────────────────────────────────────────────────────┘
```

## Event Types

Events are plain data structs that carry information. They don't have an `execute()` method like Commands.

```cpp
struct TriggerCollisionEvent {
    helios::util::Guid entityA;
    helios::util::Guid entityB;
    helios::math::vec3f contactPoint;
};

struct SpawnRequestEvent {
    EnemyType type;
    helios::math::vec3f position;
};
```

## Phase/Pass Architecture

The helios game loop is organized into **Phases** and **Passes**:

```
┌─────────────────────────────────────────────────────────────────────┐
│                            FRAME                                    │
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│  ┌─────────────────────────────────────────────────────────────┐    │
│  │                     PRE PHASE                               │    │
│  │  ┌─────────┐   ┌─────────┐   ┌─────────┐                    │    │
│  │  │ Pass 1  │──>│ Pass 2  │──>│ Pass 3  │                    │    │
│  │  │ (Input) │   │(Commit) │   │(Physics)│                    │    │
│  │  └─────────┘   └────┬────┘   └─────────┘                    │    │
│  │                     │                                        │    │
│  │              Pass Commit Point                               │    │
│  └─────────────────────────────────────────────────────────────┘    │
│                            │                                        │
│                     Phase Commit                                    │
│                            ▼                                        │
│  ┌─────────────────────────────────────────────────────────────┐    │
│  │                    MAIN PHASE                               │    │
│  │  ┌─────────┐   ┌─────────┐   ┌─────────┐                    │    │
│  │  │ Pass 1  │──>│ Pass 2  │──>│ Pass 3  │                    │    │
│  │  │(Gameplay│   │(Collision│  │ (AI)   │                    │    │
│  │  └─────────┘   └─────────┘   └─────────┘                    │    │
│  └─────────────────────────────────────────────────────────────┘    │
│                            │                                        │
│                     Phase Commit                                    │
│                            ▼                                        │
│  ┌─────────────────────────────────────────────────────────────┐    │
│  │                    POST PHASE                               │    │
│  │  ┌─────────┐   ┌─────────┐                                  │    │
│  │  │ Pass 1  │──>│ Pass 2  │                                  │    │
│  │  │ (Sync)  │   │(Cleanup)│                                  │    │
│  │  └─────────┘   └─────────┘                                  │    │
│  └─────────────────────────────────────────────────────────────┘    │
│                            │                                        │
│                     Phase Commit                                    │
│                            ▼                                        │
│                        RENDER                                       │
└─────────────────────────────────────────────────────────────────────┘
```

### Phases

A **Phase** is a major segment of the game loop with a specific purpose:

| Phase | Purpose |
|-------|---------|
| **Pre** | Input processing, event dispatch, command execution |
| **Main** | Core gameplay: physics, AI, collision, gameplay systems |
| **Post** | Scene synchronization, cleanup, state reset |

After each phase completes, a **Phase Commit** occurs:
- Phase event bus is swapped (events become readable in next phase)
- Pass event bus is cleared
- CommandBuffer is flushed
- Managers are flushed

### Passes

A **Pass** is a sub-unit within a phase containing one or more systems. Passes execute sequentially within their phase.

A pass can optionally have a **Commit Point**. When a pass has a commit point:
- The pass event bus is swapped after the pass completes
- Events pushed via `pushPass()` become readable in subsequent passes via `readPass()`

## Event Buses

### Phase Event Bus

The **Phase Event Bus** enables communication across phase boundaries.

```cpp
// In Pre Phase - System pushes an event
updateContext.pushPhase<SpawnRequestEvent>(EnemyType::Grunt, position);

// In Main Phase - System reads events from previous phase
for (const auto& evt : updateContext.readPhase<SpawnRequestEvent>()) {
    spawnManager.queueSpawn(evt.type, evt.position);
}
```

**Lifecycle:**
- Events pushed via `pushPhase()` are written to the write buffer
- At Phase Commit, buffers are swapped
- Events become readable via `readPhase()` in the next phase
- **Pass-level events do NOT propagate to the next phase** (cleared at phase commit)

### Pass Event Bus

The **Pass Event Bus** enables communication within a phase, across passes.

```cpp
// In Pass 1 - Collision system pushes event
updateContext.pushPass<TriggerCollisionEvent>(entityA, entityB, contact);

// Pass 1 ends with commit point

// In Pass 2 - Response system reads events
for (const auto& evt : updateContext.readPass<TriggerCollisionEvent>()) {
    // React to collision
}
```

**Lifecycle:**
- Events pushed via `pushPass()` are written to the write buffer
- At Pass Commit (if pass has commit point), buffers are swapped
- Events become readable via `readPass()` in subsequent passes
- All pass events are cleared at the next Phase Commit

## Commit Points

### Pass Commit Point

A pass can be configured with a commit point:

```cpp
auto& mainPhase = gameLoop.phase(PhaseType::Main);

// Add a pass with a commit point
mainPhase.addPass()
    .addSystem<CollisionSystem>(gameWorld)
    .addCommitPoint();  // Events from this pass become readable

mainPhase.addPass()
    .addSystem<CollisionResponseSystem>(gameWorld);  // Can read collision events
```

### Phase Commit

Phase commit happens automatically after each phase and performs:

```cpp
void phaseCommit(GameWorld& gameWorld, UpdateContext& updateContext) {
    // 1. Swap phase event bus - events become readable in next phase
    phaseEventBus_.swapBuffers();
    
    // 2. Clear pass event bus - pass events don't cross phases
    passEventBus_.clearAll();
    
    // 3. Flush commands
    commandBuffer_.flush(updateContext.gameWorld());
    
    // 4. Flush managers
    gameWorld.flushManagers(updateContext);
}
```

## Event Propagation Summary

```
┌─────────────────────────────────────────────────────────────────────┐
│                     EVENT VISIBILITY RULES                          │
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│  pushPass() events:                                                 │
│    ├── Visible in subsequent passes (after commit point)            │
│    ├── Within the SAME phase only                                   │
│    └── Cleared at phase boundary                                    │
│                                                                     │
│  pushPhase() events:                                                │
│    ├── Visible in the NEXT phase                                    │
│    ├── NOT visible in the same phase                                │
│    └── Cleared after being read (next phase commit)                 │
│                                                                     │
└─────────────────────────────────────────────────────────────────────┘
```

## Best Practices

### Event Design

- Events should be **immutable** data structs
- Keep events **lightweight** — store only essential data (IDs, not full objects)
- Use **value types** rather than pointers in events

### Choosing pushPass vs pushPhase

| Use `pushPass()` when... | Use `pushPhase()` when... |
|--------------------------|---------------------------|
| Event needs same-phase processing | Event triggers next-phase logic |
| Collision → Response within Main phase | Spawn request → Command generation |
| Tight coupling within a phase | Loose coupling across phases |

### Event vs Command

| Aspect | Event | Command |
|--------|-------|---------|
| Purpose | Signal/notification | World mutation |
| Has execute()? | No | Yes |
| Timing | Deferred (buffered) | Deferred (buffered) |
| Producer knows consumer? | No | No (with dispatchers) |

## Game Loop Integration

Full frame execution order:

```cpp
for (phase : {Pre, Main, Post}) {
    
    for (pass : phase.passes()) {
        
        for (system : pass.systems()) {
            system.update(updateContext);
            // Systems can:
            // - pushPass<E>() for same-phase communication
            // - pushPhase<E>() for next-phase communication
            // - readPass<E>() for events from previous passes
            // - readPhase<E>() for events from previous phase
        }
        
        if (pass.hasCommitPoint()) {
            passEventBus.swapBuffers();  // Pass events become readable
        }
    }
    
    // Phase Commit
    phaseEventBus.swapBuffers();    // Phase events become readable
    passEventBus.clearAll();         // Clear pass events
    commandBuffer.flush();           // Execute commands
    gameWorld.flushManagers();       // Process manager queues
}
```

## Related Modules

- `helios.engine.runtime.messaging.event.GameLoopEventBus` — Double-buffered event bus
- `helios.engine.runtime.world.UpdateContext` — Event push/read methods
- `helios.engine.runtime.gameloop.GameLoop` — Phase/pass orchestration
- `helios.engine.runtime.gameloop.Phase` — Phase container
- `helios.engine.runtime.gameloop.Pass` — Pass container with optional commit point
- `helios.core.buffer.TypeIndexedDoubleBuffer` — Underlying buffer implementation

