# helios::engine::runtime::spawn::events

Spawn-related events for cross-frame communication.

## Overview

This module provides events that are pushed to the frame event bus to enable communication between the spawn system and the scheduling system.

## Key Classes

| Class | Purpose |
|-------|---------|
| `SpawnPlanCommandExecutedEvent` | Signals that a spawn plan command was executed |

## Usage

Events are pushed by the SpawnManager after processing spawn commands:

```cpp
// In SpawnManager::flush()
updateContext.pushFrame<SpawnPlanCommandExecutedEvent>(
    spawnRuleId, actualSpawnCount
);
```

Events are read by the GameObjectSpawnSystem in the next frame:

```cpp
// In GameObjectSpawnSystem::update()
for (const auto& evt : updateContext.readFrame<SpawnPlanCommandExecutedEvent>()) {
    spawnScheduler_->commit(evt.spawnRuleId, evt.spawnCount);
}
```

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::runtime::spawn::events
@brief Spawn-related events for cross-frame communication.
@details Contains events pushed to the frame event bus to signal spawn command completion and enable scheduler state updates.
</p></details>
