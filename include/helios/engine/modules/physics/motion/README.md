# helios::engine::modules::physics::motion

Motion physics components and systems.

This module provides components and systems for entity movement including velocity, heading, and rotation physics.

## Submodules

| Submodule | Purpose |
|-----------|---------|
| `components/` | Movement state and configuration data |
| `systems/` | Velocity integration and steering logic |
| `commands/` | Movement and heading commands |

## Key Classes

| Class | Purpose |
|-------|---------|
| `Move2DComponent` | Velocity, speed, acceleration, and throttle state |
| `DirectionComponent` | Normalized movement direction |
| `SteeringComponent` | Rotation speed and steering behavior flags |
| `Move2DSystem` | Applies velocity to translation each frame |
| `SteeringSystem` | Rotates heading toward steering input |

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::modules::physics::motion
@brief Motion physics components and systems.
@details This namespace contains the motion layer of the physics system. It provides velocity-based movement, heading control, and rotation physics.
</p></details>

