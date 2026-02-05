# helios::engine::modules::physics::collision::systems

Collision detection and response systems for the helios engine.

This namespace contains systems for spatial partitioning, collision detection, and collision response handling. The systems are designed to run in a specific order within the game loop:

1. **BoundsUpdateSystem** - Updates world-space AABB colliders from entity transforms.
2. **GridCollisionDetectionSystem** - Performs broad-phase and narrow-phase collision detection using uniform spatial partitioning.
3. **CollisionStateResponseSystem** - Processes collision states and issues response commands (e.g., despawn).
4. **CollisionStateClearSystem** - Clears collision state components at frame end (post-phase).

## GridCollisionDetectionSystem

Implements grid-based spatial partitioning for efficient collision detection:

- **Broadphase:** Entities are inserted into grid cells based on their AABB
- **Narrowphase:** AABB intersection tests for entities sharing the same cell
- **Layer Filtering:** Uses layer masks for fine-grained collision pair control
- **Hit Policy:** Supports `OneHit` (first collision only) or `All` (multi-target)

## CollisionStateResponseSystem

Processes collision states and executes configured behaviors:

- Reads `CollisionStateComponent` to determine collision response
- Issues `DespawnCommand` for entities with `Despawn` behavior
- Publishes `SolidCollisionEvent` / `TriggerCollisionEvent` for `PassEvent` behavior

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::modules::physics::collision::systems
@brief Collision detection and response systems.
@details This namespace contains systems for updating collider bounds, performing grid-based spatial partitioning for efficient broad-phase detection, processing collision responses, and clearing collision state at frame end.
</p></details>

