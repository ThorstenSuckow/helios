# Command System

This document describes the Command pattern implementation in helios, including command buffering, dispatching, and integration with the game loop.

## Overview

The Command System provides a mechanism for **deferred action execution**. Instead of immediately modifying game state, actions are encapsulated as Command objects, buffered, and executed in a controlled batch during the game loop. This decouples input handling from action processing and enables deterministic, reproducible game logic.

```
┌─────────────────┐     ┌─────────────────┐     ┌─────────────────┐
│  Input System   │────>│  CommandBuffer  │────>│    GameWorld    │
│  (produces)     │     │  (queues)       │     │  (receives)     │
└─────────────────┘     └─────────────────┘     └─────────────────┘
                              │
                              │ flush()
                              ▼
                        ┌─────────────────┐
                        │   Dispatchers   │
                        │   (route)       │
                        └─────────────────┘
                              │
                              ▼
                        ┌─────────────────┐
                        │    Managers     │
                        │   (execute)     │
                        └─────────────────┘
```

## Command Types

### TargetedCommand

A `TargetedCommand` operates on a **specific GameObject** identified by its `Guid`. Use this for per-entity actions like movement, attacks, or state changes.

```cpp
class MoveCommand : public TargetedCommand {
    helios::math::vec3f direction_;
    float speed_;
    
public:
    MoveCommand(helios::math::vec3f dir, float speed)
        : direction_(dir), speed_(speed) {}

    void execute(helios::engine::ecs::GameObject& obj) const noexcept override {
        auto* move = obj.get<Move2DComponent>();
        if (move) {
            move->setVelocity(direction_ * speed_);
        }
    }
};
```

### WorldCommand

A `WorldCommand` operates on the **entire GameWorld** rather than a specific entity. Use this for global actions like spawning entities, level transitions, or world-wide effects.

```cpp
class SpawnEnemyCommand : public WorldCommand {
    helios::math::vec3f position_;
    
public:
    explicit SpawnEnemyCommand(helios::math::vec3f pos) : position_(pos) {}

    void execute(helios::engine::runtime::world::GameWorld& world) const noexcept override {
        auto enemy = std::make_unique<helios::engine::ecs::GameObject>();
        // Configure enemy...
        world.addGameObject(std::move(enemy));
    }
};
```

## CommandBuffer

The `CommandBuffer` is a queue that collects commands during the frame and executes them in batch via `flush()`.

### Adding Commands

```cpp
CommandBuffer cmdBuffer;

// Add a TargetedCommand (requires Guid)
cmdBuffer.add<MoveCommand>(player.guid(), direction, speed);

// Add a WorldCommand
cmdBuffer.add<SpawnEnemyCommand>(spawnPosition);
```

### Flushing Commands

During `flush()`, commands are executed in a defined order:

1. **WorldCommands** are processed first (global state changes)
2. **TargetedCommands** are processed second (per-entity changes)

```cpp
// At end of input phase or before simulation
cmdBuffer.flush(gameWorld);
```

After flush, the buffer is automatically cleared.

## Dispatchers

Dispatchers enable the **Visitor pattern** for type-safe command routing. Instead of executing commands directly, dispatchers route them to specialized handlers (e.g., Managers).

### Registering Dispatchers

```cpp
// Create a typed dispatcher for DespawnCommand
class DespawnDispatcher : public TypedWorldCommandDispatcher<DespawnCommand> {
    SpawnManager& manager_;
public:
    explicit DespawnDispatcher(SpawnManager& mgr) : manager_(mgr) {}

    void dispatchTyped(GameWorld& world, const DespawnCommand& cmd) noexcept override {
        manager_.submit(cmd);
    }
};

// Register with CommandBuffer
cmdBuffer.addDispatcher<DespawnCommand>(
    std::make_unique<DespawnDispatcher>(spawnManager)
);
```

### Dispatch Flow

When `flush()` encounters a command with a registered dispatcher:

1. The command's `accept()` method is called with the dispatcher
2. The dispatcher's `dispatch()` method receives the command
3. `TypedDispatcher` downcasts and calls `dispatchTyped()` with full type info

```
Command.accept(dispatcher)
    │
    ▼
Dispatcher.dispatch(command)
    │
    ▼
TypedDispatcher.dispatchTyped(typedCommand)
    │
    ▼
Manager.queueRequest(...)
```

## Managers

Managers handle cross-cutting concerns that require deferred or batched processing. Commands route to Managers via Dispatchers.

### Manager Lifecycle

```cpp
class SpawnManager : public Manager, public SpawnCommandHandler {
    std::vector<SpawnCommand> spawnCommands_;
    std::vector<DespawnCommand> despawnCommands_;

public:
    bool submit(const SpawnCommand& cmd) noexcept override {
        spawnCommands_.push_back(cmd);
        return true;
    }

    bool submit(const DespawnCommand& cmd) noexcept override {
        despawnCommands_.push_back(cmd);
        return true;
    }

    // Called after CommandBuffer::flush()
    void flush(GameWorld& world, UpdateContext& ctx) noexcept override {
        for (const auto& cmd : spawnCommands_) {
            // Acquire from pool and initialize
        }
        for (const auto& cmd : despawnCommands_) {
            // Return to pool
        }
        spawnCommands_.clear();
        despawnCommands_.clear();
    }
};
```

### Registration

Managers are registered with the GameWorld:

```cpp
auto spawnManager = std::make_unique<SpawnManager>();
gameWorld.addManager(std::move(spawnManager));
```

## Game Loop Integration

The Command System integrates with the Phase/Pass game loop architecture. Commands can be added during **any phase**, and are flushed at each **Phase Commit**.

```
┌─────────────────────────────────────────────────────────────────────┐
│                            FRAME                                    │
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│  PRE PHASE ──────────────────────────────────────────────────────   │
│    Pass 1 (Input): Systems read input, add Commands                 │
│        cmdBuffer.add<MoveCommand>(guid, dir, spd);                  │
│    Pass 2, Pass 3, ...                                              │
│  ────────────────────────────────────────────────── Phase Commit    │
│       │  1. phaseEventBus.swapBuffers()                             │
│       │  2. passEventBus.clearAll()                                 │
│       │  3. commandBuffer.flush()      ◄── Commands execute here    │
│       │  4. gameWorld.flushManagers()  ◄── Managers process queues  │
│       ▼                                                             │
│  MAIN PHASE ─────────────────────────────────────────────────────   │
│    Pass 1 (Gameplay): Movement, Physics systems                     │
│    Pass 2 (Collision): Collision detection                          │
│           Pass 2 commit!                                            │
│    Pass 3 (AI): AI systems                                          │
│           (Access to events from Pass 1/2                           │
│           due to Pass 2 commit)                                     │
│        Commands can still be added here!                            │
│  ────────────────────────────────────────────────── Phase Commit    │
│       │  commandBuffer.flush() + gameWorld.flushManagers()          │
│       ▼                                                             │
│  POST PHASE ─────────────────────────────────────────────────────   │
│    Pass 1 (Scene Sync): Sync transforms to scene graph              │
│    Pass 2 (Cleanup): Clear dirty flags                              │
│  ────────────────────────────────────────────────── Phase Commit    │
│       │  commandBuffer.flush() + gameWorld.flushManagers()          │
│       ▼                                                             │
│                          RENDER                                     │
└─────────────────────────────────────────────────────────────────────┘
```

### Key Points

Commands are **not** limited to the Pre Phase. Systems in any phase can add commands:

```cpp
void update(UpdateContext& ctx) noexcept override {
    // Add command during any phase
    ctx.commandBuffer().add<DespawnCommand>(entity.guid(), poolId);
}
```

At each Phase Commit, the following sequence occurs:

```cpp
// Phase Commit sequence
phaseEventBus.swapBuffers();    // Phase events become readable
passEventBus.clearAll();         // Pass events are cleared
commandBuffer.flush();           // Commands execute (world mutations)
gameWorld.flushManagers();       // Managers process queued requests
```

This means spawns and despawns triggered in one phase are visible to systems in the next phase.

For detailed phase/pass event handling, see [Game Loop Architecture](gameloop-architecture.md).

## Best Practices

### Command Design

- Commands should be **immutable** after construction
- `execute()` must be **noexcept** — handle errors internally
- Keep commands **lightweight** — store only essential data
- Use `WorldCommand` for spawning; use `TargetedCommand` for entity mutations

### Dispatcher Usage

- Register dispatchers for commands that require **external handling**
- Use dispatchers to route to **Managers** for pooled entity lifecycle
- Direct `execute()` is fine for simple, self-contained commands

### Manager Integration

- Managers **queue** requests during command dispatch
- Managers **execute** requests during `flushManagers()`
- This ensures all commands are processed before entities are spawned/despawned

## Related Modules

- `helios.engine.runtime.messaging.command.CommandBuffer` — Central command queue
- `helios.engine.runtime.messaging.command.TargetedCommand` — Per-entity command base
- `helios.engine.runtime.messaging.command.WorldCommand` — World-wide command base
- `helios.engine.runtime.messaging.command.TypedTargetedCommandDispatcher` — Type-safe entity command dispatch
- `helios.engine.runtime.messaging.command.TypedWorldCommandDispatcher` — Type-safe world command dispatch
- `helios.engine.runtime.world.Manager` — Base class for deferred processing managers
- `helios.engine.runtime.world.GameWorld` — Manager registration and flushing

## Related Documentation

- [Event System](event-system.md) — Phase/pass event propagation
- [Game Loop Architecture](gameloop-architecture.md) — Overall frame structure
- [Component System](component-system.md) — GameObject, Component, System architecture
- [Spawn System](spawn-system.md) — Entity lifecycle with spawn scheduling and pooling

