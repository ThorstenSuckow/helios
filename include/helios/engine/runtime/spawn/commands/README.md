# helios::engine::runtime::spawn::commands

Commands for spawn and despawn operations.

## Overview

This module contains command classes that represent spawn and despawn requests. These commands are emitted by systems and processed by dispatchers during the command flush phase.

## Key Classes

| Command | Purpose |
|---------|---------|
| `SpawnCommand` | Requests spawning of GameObjects from a pool with spawn context |
| `DespawnCommand` | Requests despawning and pool return of a GameObject |
| `ScheduledSpawnPlanCommand` | Executes a scheduled spawn plan from the SpawnScheduler |

## Architecture

Commands follow the visitor pattern with dispatchers:

```
┌─────────────────┐     ┌───────────────┐     ┌────────────┐     ┌──────────────┐
│ SpawnSystem     │────>│ CommandBuffer │────>│ Dispatcher │────>│ SpawnManager │
│                 │     │               │     │            │     │              │
│ - SpawnCommand  │     │ flush()       │     │ dispatch() │     │ submit()     │
│ - DespawnCommand│     │               │     │            │     │              │
└─────────────────┘     └───────────────┘     └────────────┘     └──────────────┘
```

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
@namespace helios::engine::runtime::spawn::commands
@brief Commands for spawn and despawn operations.
@details This namespace contains command classes for deferred spawn and despawn operations. Commands are processed during the command buffer flush phase and routed to managers via dispatchers.
</p></details>

