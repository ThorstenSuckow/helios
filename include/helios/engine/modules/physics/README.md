# helios::engine::modules::physics

Physics simulation and collision detection subsystem for the game engine.

This namespace provides the complete physics layer for the helios game engine, including movement simulation, collision detection, and physics-related event handling.

## Submodules

| Submodule | Purpose |
|-----------|---------|
| `collision/` | AABB collision detection with spatial partitioning |
| `motion/` | Velocity-based movement and steering systems |

## Key Classes

### Collision

| Class | Purpose |
|-------|---------|
| `CollisionComponent` | Layer ID, collision mask, and behavior per collider |
| `AabbColliderComponent` | World-space AABB for broad-phase detection |
| `CollisionStateComponent` | Per-frame collision results (solid/trigger hits) |
| `GridCollisionDetectionSystem` | Spatial-hash broad-phase + narrow-phase collision |
| `BoundsUpdateSystem` | Updates world-space AABBs from transforms |
| `CollisionStateResponseSystem` | Executes collision behaviors (despawn, bounce) |

### Motion

| Class | Purpose |
|-------|---------|
| `Move2DComponent` | Velocity, speed, acceleration, damping config |
| `DirectionComponent` | Normalized direction vector |
| `SteeringComponent` | Rotation speed and steering behavior config |
| `Move2DSystem` | Integrates velocity and updates translation |
| `SteeringSystem` | Updates heading from steering input |

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::modules::physics
@brief Physics simulation and collision detection subsystem for the game engine.
@details This namespace provides the complete physics layer for the helios game engine, including movement simulation, collision detection, and physics-related event handling. The architecture follows a data-driven design where components store state and systems process behavior.
</p></details>

