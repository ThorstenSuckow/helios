# helios::engine::modules::physics::collision

Collision detection components, events, and systems.
This module provides the collision detection infrastructure including collider components, collision events, and detection systems.

## Submodules

| Submodule | Purpose |
|-----------|---------|
| `components/` | Collider and collision state data |
| `systems/` | Detection, response, and state management |
| `events/` | Solid and trigger collision events |
| `types/` | Enums for collision behavior and response |

## Key Classes

| Class | Purpose |
|-------|---------|
| `CollisionComponent` | Layer ID, mask, and per-layer collision behavior |
| `AabbColliderComponent` | World-space bounding box for broad-phase |
| `CollisionStateComponent` | Stores collision results for the current frame |
| `GridCollisionDetectionSystem` | Grid-based spatial hash for O(n) detection |
| `BoundsUpdateSystem` | Syncs AABB from transform each frame |
| `CollisionStateClearSystem` | Clears collision state after response |
| `CollisionStateResponseSystem` | Executes behaviors (despawn, bounce, reflect) |

---

<details>
<summary>Doxygen</summary><p>
@details This namespace contains the collision detection layer of the physics system. It provides AABB-based collision detection with layer filtering and spatial partitioning.
@brief Collision detection components, events, and systems.
@namespace helios::engine::modules::physics::collision
</p></details>