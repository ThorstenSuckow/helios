# helios::engine::runtime::spawn::dispatcher

Command dispatchers for spawn operations.

## Overview

This module contains dispatchers that route spawn and despawn commands to the appropriate managers for processing. Dispatchers implement the visitor pattern, allowing the CommandBuffer to route commands without knowing their concrete types.

## Key Classes

| Dispatcher | Handles | Purpose |
|------------|---------|---------|
| `SpawnCommandDispatcher` | `SpawnCommand` | Routes spawn commands to SpawnManager |
| `DespawnCommandDispatcher` | `DespawnCommand` | Routes despawn commands to SpawnManager |
| `ScheduledSpawnPlanCommandDispatcher` | `ScheduledSpawnPlanCommand` | Routes scheduled plans to SpawnManager |

## Architecture

```
┌─────────────────┐     ┌───────────────┐     ┌──────────────────────────────────┐
│ CommandBuffer   │     │ Dispatcher    │     │ SpawnCommandHandler              │
│                 │     │               │     │ (SpawnManager)                   │
│ flush()         │────>│ dispatchTyped │────>│ submit(SpawnCommand)             │
│                 │     │               │     │ submit(DespawnCommand)           │
│                 │     │               │     │ submit(ScheduledSpawnPlanCommand)│
└─────────────────┘     └───────────────┘     └──────────────────────────────────┘
```

Dispatchers are registered with the CommandBuffer during game initialization:

```cpp
commandBuffer.addDispatcher<SpawnCommandDispatcher>();
commandBuffer.addDispatcher<DespawnCommandDispatcher>();
commandBuffer.addDispatcher<ScheduledSpawnPlanCommandDispatcher>();
```

## Usage

Commands are added to the buffer by systems and automatically routed during flush:

```cpp
// In a system
commandBuffer.add<SpawnCommand>(profileId, spawnContext, count);
commandBuffer.add<DespawnCommand>(entityGuid, profileId);

// During flush, dispatchers route commands to handlers
commandBuffer.flush(gameWorld);
```

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::runtime::spawn::dispatcher
@brief Command dispatchers for spawn operations.
@details This namespace contains typed command dispatchers that route spawn and despawn commands to the SpawnManager for deferred processing via SpawnCommandHandler.
</p></details>

