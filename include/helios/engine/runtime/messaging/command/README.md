# helios::engine::runtime::messaging::command

Compile-time typed command buffering and handler routing infrastructure.

## Overview

This module implements the Command pattern for deferred action execution within the game loop. Commands are enqueued during system updates and flushed at defined commit points, ensuring deterministic and reproducible world mutations.

The system is built around **compile-time type safety**: command types are declared as template parameters, eliminating virtual dispatch overhead for queue access and enabling the compiler to verify command routing at build time.

## Architecture

```
┌──────────────────────────────────────────────────────────────────────┐
│                     COMMAND PIPELINE                                 │
├──────────────────────────────────────────────────────────────────────┤
│                                                                      │
│  SYSTEMS (producers)                                                 │
│  ┌────────────────────────────────────────────────────────────┐      │
│  │  ctx.queueCommand<MoveCommand>(entityHandle, dir);  │      │
│  │  ctx.queueCommand<DespawnCommand>(eh, profileId);   │      │
│  └───────────────────────────┬────────────────────────────────┘      │
│                              │                                       │
│                              ▼                                       │
│  BUFFER (EngineCommandBuffer wrapping TypedCommandBuffer)            │
│  ┌────────────────────────────────────────────────────────────┐      │
│  │  tuple< vector<Aim2DCmd>, vector<ShootCmd>, ... >          │      │
│  └───────────────────────────┬────────────────────────────────┘      │
│                              │ flush()                               │
│                              ▼                                       │
│  ROUTING (per command type)                                          │
│  ┌────────────────────────────────────────────────────────────┐      │
│  │  Handler registered in CommandHandlerRegistry?             │      │
│  │    YES → registry.submit<Cmd>(cmd) via function pointer    │      │
│  │    NO  → cmd.execute(updateContext)  [if ExecutableCommand]│      │
│  └───────────────────────────┬────────────────────────────────┘      │
│                              │                                       │
│                              ▼                                       │
│  MANAGERS (consumers)                                                │
│  ┌────────────────────────────────────────────────────────────┐      │
│  │  SpawnManager, ScorePoolManager, TimerManager, ...         │      │
│  │  → Manager::flush() processes queued requests              │      │
│  └────────────────────────────────────────────────────────────┘      │
│                                                                      │
└──────────────────────────────────────────────────────────────────────┘
```

## Key Classes

| Class | Purpose |
|-------|---------|
| `CommandBuffer` | Abstract base for command buffers |
| `CommandHandlerRegistry` | Function-pointer based registry for command handlers |
| `CommandHandlerEntry` | Type-erased entry storing owner pointer and submit function |
| `CommandHandlerRef<T>` | Typed reference wrapper for invoking registered handlers |
| `TypedCommandBuffer<...Cmds>` | Compile-time typed buffer with per-type queues |
| `EngineCommandBuffer` | Concrete facade pre-configured with all engine command types |

## Flush Routing

During `TypedCommandBuffer::flush()`, each command type is processed in template parameter order:

1. **Handler route:** If a handler is registered in the `CommandHandlerRegistry`, each queued command is submitted via the stored function pointer.
2. **Direct execution:** If no handler is registered and the command satisfies the `ExecutableCommand` concept (provides a noexcept `execute(UpdateContext&)` method), it is executed directly.
3. **Assertion:** If neither condition holds, an assertion fires (misconfiguration).

## Usage

```cpp
// Systems enqueue commands via UpdateContext
void update(UpdateContext& ctx) noexcept {
    ctx.queueCommand<DespawnCommand>(entityHandle, profileId);
}

// Managers provide submit() methods for commands they process
class SpawnManager {
public:
    using EngineRoleTag = helios::engine::common::tags::ManagerRole;

    bool submit(const SpawnCommand& cmd) noexcept {
        spawnQueue_.push_back(cmd);
        return true;
    }

    bool submit(const DespawnCommand& cmd) noexcept {
        despawnQueue_.push_back(cmd);
        return true;
    }

    // Register handlers during init()
    void init(GameWorld& gameWorld) {
        gameWorld.registerCommandHandler<SpawnCommand>(*this);
        gameWorld.registerCommandHandler<DespawnCommand>(*this);
    }

    void flush(UpdateContext& ctx) noexcept { /* batch processing */ }
};
```

## Game Loop Integration

Commands are flushed at each commit point in the game loop:

```cpp
// Phase commit sequence
commandBuffer.flush(gameWorld, updateContext); // Commands route to handlers
gameWorld.flushManagers(updateContext);         // Managers process queued requests
```

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::runtime::messaging::command
@brief Compile-time typed command buffering and handler routing.
@details Provides deferred command execution with type-safe queues, handler-or-execute routing, and integration with the Manager flush cycle.
</p></details>

