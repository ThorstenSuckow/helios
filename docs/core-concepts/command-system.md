# Command System

This document describes the Command pattern implementation in helios, including compile-time typed command buffering, handler routing, and integration with the game loop.

## Overview

The Command System provides a mechanism for **deferred action execution**. Instead of immediately modifying game state, actions are encapsulated as lightweight command structs, buffered in compile-time typed queues, and executed in a controlled batch during the game loop. This decouples input handling from action processing and enables deterministic, reproducible game logic.

```
┌─────────────────┐     ┌──────────────────────┐     ┌─────────────────┐
│  Input / Systems│────>│ EngineCommandBuffer  │────>│   Managers      │
│  (produce cmds) │     │ (typed queues)       │     │  (process)      │
└─────────────────┘     └──────────────────────┘     └─────────────────┘
                               │ flush()
                               ▼
                        ┌──────────────────────┐
                        │   Registered Handler │
                        │  (via function ptr)  │
                        └──────────────────────┘
```

## Command Types

Commands are plain structs (value types) that carry the data needed for a specific action. They do not inherit from a common base class — instead, the compile-time typed buffer ensures type safety.

### Handler-Routed Commands

Most commands are routed to a Manager's registered submit function during flush:

```cpp
struct DespawnCommand {
    SpawnProfileId spawnProfileId_;
    EntityHandle entityHandle_;

    SpawnProfileId spawnProfileId() const noexcept { return spawnProfileId_; }
    EntityHandle entityHandle() const noexcept { return entityHandle_; }
};
```

### Self-Executing Commands

Commands can also execute themselves if they satisfy the `ExecutableCommand` concept. During `flush()`, the `TypedCommandBuffer` resolves the handler via the `GameWorld`'s `ResourceRegistry` internally — self-executing commands receive the `UpdateContext` directly:

```cpp
struct StateCommand {
    StateTransitionContext context_;

    void execute(UpdateContext& ctx) const noexcept {
        // Self-executing commands receive the UpdateContext;
        // handler routing is resolved internally by TypedCommandBuffer
    }
};
```

## TypedCommandBuffer

`TypedCommandBuffer<...CommandTypes>` is the core of the system. It stores commands in a `std::tuple<std::vector<CommandType>...>`, providing one contiguous queue per command type.

### Adding Commands

```cpp
// Systems add commands via UpdateContext::queueCommand<T>()
ctx.queueCommand<Move2DCommand>(entityHandle, direction, speed);
ctx.queueCommand<DespawnCommand>(entityHandle, profileId);
```

### Flush Routing

During `flush()`, each command type is processed in template parameter order:

1. **Handler route:** If a handler for `Cmd` is registered in the `CommandHandlerRegistry`, all queued commands are submitted to the handler via the stored function pointer.
2. **Direct execution:** Otherwise, if the command satisfies `ExecutableCommand`, it executes itself
3. **Assertion:** If neither applies, an assertion fires (misconfiguration)

```cpp
// Pseudocode for flush routing (per command type):
if (registry.has<Cmd>()) {
    auto& handlerRef = registry.tryHandler<Cmd>();
    for (auto& cmd : queue) {
        handlerRef.submit(cmd);
    }
} else if constexpr (ExecutableCommand<Cmd>) {
    for (auto& cmd : queue) {
        cmd.execute(updateContext);
    }
}
queue.clear();
```

### Deterministic Ordering

Commands are flushed in the order of the template parameter list. The `EngineCommandBuffer` defines this order:

1. Combat commands (Aim2D, Shoot)
2. Motion commands (Move2D, Steering)
3. Scoring commands (UpdateScore)
4. Spawn commands (ScheduledSpawnPlan, Spawn, Despawn)
5. State commands (GameState, MatchState)
6. UI commands (UiAction)
7. Timing commands (TimerControl)

## Command Handlers

Most of the Managers in helios process commands by providing a `submit` method for each command type they handle. Concrete managers are plain classes — they do not inherit from any handler interface. Instead, they declare `using EngineRoleTag = ManagerTag;` to opt in:

```cpp
class SpawnManager {
public:
    using EngineRoleTag = helios::engine::common::tags::ManagerTag;

    bool submit(const SpawnCommand& cmd) noexcept {
        spawnCommands_.push_back(cmd);
        return true;
    }

    bool submit(const DespawnCommand& cmd) noexcept {
        despawnCommands_.push_back(cmd);
        return true;
    }

    bool submit(ScheduledSpawnPlanCommand cmd) noexcept {
        scheduledCommands_.push_back(cmd);
        return true;
    }

    void flush(UpdateContext& ctx) noexcept { /* batch processing */ }
};
```

Handlers are registered with the `ResourceRegistry` (via `GameWorld` convenience methods) during `init()`:

```cpp
void init(GameWorld& gameWorld) {
    gameWorld.registerCommandHandler<SpawnCommand>(*this);
    gameWorld.registerCommandHandler<DespawnCommand>(*this);
}
```

## EngineCommandBuffer

`EngineCommandBuffer` is a thin facade over `TypedCommandBuffer` instantiated with all engine command types. It is registered as a resource in the `GameWorld` and used internally by `UpdateContext::queueCommand<T>()`.

```cpp
class EngineCommandBuffer : public CommandBuffer {
    using BufferImpl = TypedCommandBuffer<
        Aim2DCommand, ShootCommand,
        Move2DCommand, SteeringCommand,
        UpdateScoreCommand,
        ScheduledSpawnPlanCommand, SpawnCommand, DespawnCommand,
        StateCommand<GameState>, StateCommand<MatchState>,
        UiActionCommand,
        TimerControlCommand
    >;

    BufferImpl impl_;

public:
    template<class T, class... Args>
    void add(Args&&... args) {
        impl_.add<T>(std::forward<Args>(args)...);
    }

    void flush(GameWorld& gameWorld, UpdateContext& ctx) noexcept override {
        impl_.flush(gameWorld, ctx);
    }
    void clear() noexcept override { impl_.clear(); }
};
```

## Managers

Managers handle cross-cutting concerns that require deferred or batched processing. Commands route to Managers via registered submit functions during the buffer flush, and Managers process their queues during `Manager::flush()`.

### Manager Lifecycle

1. **Registration:** Manager is added via `GameWorld::registerManager<T>()`
2. **init():** Called during `GameWorld::init()`. Registers command handlers with the `GameWorld`.
3. **submit():** Receives commands during `CommandBuffer::flush()`
4. **flush():** Processes queued commands during `GameWorld::flushManagers()`

### Registration

```cpp
auto& spawnManager = gameWorld.registerManager<SpawnManager>();
```

## Game Loop Integration

The Command System integrates with the Phase/Pass game loop architecture. Commands can be added during **any phase**, and are flushed at each **commit point**.

```
┌─────────────────────────────────────────────────────────────────────┐
│                            FRAME                                    │
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│  PRE PHASE ──────────────────────────────────────────────────────   │
│    Pass 1 (Input): Systems read input, add commands                 │
│        ctx.queueCommand<Move2DCommand>(dir, spd);                   │
│    Pass 2, Pass 3, ...                                              │
│  ────────────────────────────────────────────────── Phase Commit    │
│       │  1. eventBus.swapBuffers()                                  │
│       │  2. commandBuffer.flush(gameWorld, ctx) ◄─ Commands         │
│       │  3. flushManagers(ctx)                  ◄─ Managers process │
│       ▼                                            commands         │
│  MAIN PHASE ─────────────────────────────────────────────────────   │
│    Pass 1 (Gameplay): Movement, Physics systems                     │
│    Pass 2 (Collision): Collision detection                          │
│        Commands can still be added here!                            │
│  ────────────────────────────────────────────────── Phase Commit    │
│       │  commandBuffer.flush(gameWorld, ctx) + flushManagers(ctx)   │
│       ▼                                                             │
│  POST PHASE ─────────────────────────────────────────────────────   │
│    Pass 1 (Scene Sync): Sync transforms to scene graph              │
│    Pass 2 (Cleanup): Clear dirty flags                              │
│  ────────────────────────────────────────────────── Phase Commit    │
│       │  commandBuffer.flush(gameWorld, ctx) + flushManagers(ctx)   │
│       ▼                                                             │
│                          RENDER                                     │
└─────────────────────────────────────────────────────────────────────┘
```

### Key Points

- Commands are **not** limited to the Pre Phase. Systems in any phase can add commands.
- At each commit point, commands are flushed **before** managers, ensuring handlers receive all commands before processing.
- Spawns and despawns triggered in one phase are visible to systems in the next phase.

## Best Practices

### Command Design

- Commands should be **immutable value types** (structs with const accessors)
- Keep commands **lightweight** — store only essential data (IDs, handles, values)
- Use handler-routed commands for operations that require Manager coordination
- Use self-executing commands for simple state transitions

### Handler Registration

- Register handlers in `Manager::init()`, not in constructors
- Ensure the Manager is registered in the `ResourceRegistry` before dependent Managers call `init()`
- A single Manager can register handlers for multiple command types

### Ordering

- The template parameter order of `EngineCommandBuffer` determines flush order
- Design command ordering to avoid dependencies (e.g., despawns before spawns)

## Related Modules

- `helios.engine.runtime.messaging.command.CommandBuffer` — Abstract buffer base
- `helios.engine.runtime.messaging.command.TypedCommandBuffer` — Compile-time typed buffer
- `helios.engine.runtime.messaging.command.CommandHandlerRegistry` — Function-pointer based registry for command handlers
- `helios.engine.runtime.messaging.command.EngineCommandBuffer` — Concrete engine buffer
- `helios.engine.runtime.world.Manager` — Base class for deferred processing managers
- `helios.engine.runtime.world.ResourceRegistry` — Resource storage

## Related Documentation

- [Resource Registry](resource-registry.md) — Type-indexed resource storage and handler lookup
- [Event System](event-system.md) — Phase/pass event propagation
- [Game Loop Architecture](gameloop-architecture.md) — Overall frame structure
- [Component System](component-system.md) — GameObject, Component, System architecture
- [Spawn System](spawn-system.md) — Entity lifecycle with spawn scheduling and pooling
