# helios::engine::runtime::spawn::requests

Data structures for spawn and despawn requests.

## Overview

This module contains lightweight data transfer objects used to communicate spawn and despawn parameters between dispatchers and managers.

## Key Classes

| Structure | Purpose |
|-----------|---------|
| `SpawnRequest` | Contains spawn profile ID and spawn context |
| `DespawnRequest` | Contains entity GUID and optional spawn profile ID |
| `ScheduledSpawnPlanRequest` | Contains scheduled spawn plan and spawn context |

## Data Flow

```
┌─────────────────┐     ┌───────────────┐     ┌─────────────────┐
│ Command         │     │ Dispatcher    │     │ SpawnManager    │
│                 │     │               │     │                 │
│ SpawnCommand    │────>│ creates       │────>│ submit(request) │
│ DespawnCommand  │     │ Request       │     │                 │
│ ScheduledPlan   │     │               │     │                 │
└─────────────────┘     └───────────────┘     └─────────────────┘
```

## Usage

Requests are created by dispatchers and submitted to managers:

```cpp
// In SpawnCommandDispatcher
SpawnRequest request{profileId, spawnContext};
spawnRequestHandler->submit(request);

// In DespawnCommandDispatcher
DespawnRequest request{entityGuid, profileId};
spawnRequestHandler->submit(request);

// In ScheduledSpawnPlanCommandDispatcher
ScheduledSpawnPlanRequest request{scheduledPlan, spawnContext};
spawnRequestHandler->submit(request);
```

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::runtime::spawn::requests
@brief Data structures for spawn and despawn requests.
@details This namespace contains lightweight data transfer objects for communicating spawn and despawn parameters between dispatchers and the SpawnManager.
</p></details>

