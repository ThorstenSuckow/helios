# helios::engine::runtime::spawn::events

Spawn-related events for cross-frame communication.

## Overview

This module provides events that are pushed to the frame event bus to enable communication between the spawn system and the scheduling system.

## Key Classes

| Class | Purpose |
|-------|---------|
| `SpawnPlanRequestExecutedEvent` | Signals that a spawn plan was executed |

## Usage

Events are pushed by the SpawnManager after processing spawn requests:

```cpp
// In SpawnManager::flush()
updateContext.pushFrame<SpawnPlanRequestExecutedEvent>(
    spawnRuleId, actualSpawnCount
);
```

Events are read by the GameObjectSpawnSystem in the next frame:

```cpp
// In GameObjectSpawnSystem::update()
for (const auto& evt : updateContext.readFrame<SpawnPlanRequestExecutedEvent>()) {
    spawnScheduler_->commit(evt.spawnRuleId, evt.spawnCount);
}
```

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::runtime::spawn::events
@brief Spawn-related events for cross-frame communication.
@details Contains events pushed to the frame event bus to signal spawn completion and enable scheduler state updates.
</p></details>
