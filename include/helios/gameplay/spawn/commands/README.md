# helios::gameplay::spawn::commands

Commands for spawn and despawn operations.

## Overview

This module contains command classes that represent spawn and despawn operations. These commands are emitted by systems and submitted directly to the SpawnCommandHandler during the command flush phase.

The system is built around **compile-time type safety**: command types are declared as template parameters, eliminating virtual dispatch overhead for queue access and enabling the compiler to verify command routing at build time.

## Architecture

```
┌──────────────────────────────────────────────────────────────────────┐
│                     COMMAND PIPELINE                                 │
├──────────────────────────────────────────────────────────────────────┤
│                                                                      │
│  SYSTEMS (producers)                                                 │
│                                                                      │
│  - SpawnSystem                                                       │
│  - DespawnSystem                                                     │
│                                                                      │
├──────────────────────────────────────────────────────────────────────┤
│                                                                      │
│  COMMAND BUFFER (queue)                                             │
│                                                                      │
│  - CommandBuffer                                                     │
│  - TypedCommandBuffer<...Cmds>                                      │
│                                                                      │
├──────────────────────────────────────────────────────────────────────┤
│                                                                      │
│  DISPATCHER (consumer)                                              │
│                                                                      │
│  - Dispatcher                                                        │
│                                                                      │
├──────────────────────────────────────────────────────────────────────┤
│                                                                      │
│  HANDLER (execution)                                                │
│                                                                      │
│  - CommandHandlerRegistry                                            │
│  - EngineCommandBuffer                                               │
│                                                                      │
└──────────────────────────────────────────────────────────────────────┘
```

## Key Classes

| Class | Purpose |
|-------|---------|
| `CommandBuffer` | Abstract base for command buffers |
| `TypedCommandBuffer<...Cmds>` | Compile-time typed buffer with per-type queues |
| `CommandHandlerRegistry` | Registry storing typed submit functions |
| `EngineCommandBuffer` | Concrete facade pre-configured with all engine command types |

## Flush Routing

During `TypedCommandBuffer::flush()`, each command type is processed in template parameter order:

1. **Handler route:** If a handler for `Cmd` is registered in the `CommandHandlerRegistry`, all queued commands are submitted to the handler via the stored function pointer.
2. **Direct execution:** Otherwise, if the command satisfies `ExecutableCommand`, it executes itself
3. **Assertion:** If neither applies, an assertion fires (misconfiguration)

## Usage

```cpp
// Spawn command with context
SpawnContext ctx;
ctx.emitterContext = EmitterContext{position, velocity};
commandBuffer.add<SpawnCommand>(bulletProfileId, ctx, 1);

// Despawn command
commandBuffer.add<DespawnCommand>(entityGuid, profileId);

// Scheduled spawn plan command (from SpawnScheduler)
commandBuffer.add<ScheduledSpawnPlanCommand>(std::move(scheduledPlan));
```

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::gameplay::spawn::commands
@brief Commands for spawn and despawn operations.
@details This namespace contains command classes for deferred spawn and despawn operations.
</p></details>
