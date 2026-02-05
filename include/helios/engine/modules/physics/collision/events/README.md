# helios::engine::modules::physics::collision::events

Collision event types for the helios engine.

This namespace contains event structures dispatched by the collision detection system when entities collide.

## Events

| Event | Description |
|-------|-------------|
| `SolidCollisionEvent` | Emitted when a solid (physical) collision is detected |
| `TriggerCollisionEvent` | Emitted when a trigger (non-physical) collision is detected |

## Event Dispatch

Events are published via the `UpdateContext` when an entity with `PassEvent` collision behavior detects a collision. Events contain a `CollisionContext` struct with full collision details.

## Solid vs Trigger

- **Solid collisions:** Physical interactions that can block movement or trigger physics responses
- **Trigger collisions:** Non-physical interactions for gameplay logic (pickups, damage zones, area effects)

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::modules::physics::collision::events
@brief Collision event types for the helios engine.
@details Contains event structures dispatched by the collision detection system. SolidCollisionEvent represents physical collisions, while TriggerCollisionEvent represents non-blocking gameplay interactions.
</p></details>