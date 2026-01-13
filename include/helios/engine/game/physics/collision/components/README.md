# helios::engine::game::physics::collision::components

Collider components for collision detection.

This namespace contains data components that define entity collision properties used by the physics and collision detection systems.

## Components

| Component | Description |
|-----------|-------------|
| `AabbColliderComponent` | Axis-Aligned Bounding Box for broad-phase collision detection. |
| `CollisionComponent` | Layer membership and collision masks (trigger/solid). |
| `CollisionStateComponent` | Per-frame collision state written by detection systems. |

## Usage

Attach `AabbColliderComponent` to define spatial bounds and `CollisionComponent` to specify layer-based filtering. After collision detection runs, query `CollisionStateComponent` to read collision results for the current frame.

```cpp
auto& collider = gameObject.add<AabbColliderComponent>();
collider.setBounds(helios::math::aabbf{{-0.5f, -0.5f, -0.5f}, {0.5f, 0.5f, 0.5f}});

constexpr uint32_t LAYER_PLAYER   = 1 << 0;
constexpr uint32_t LAYER_ENEMY    = 1 << 1;
constexpr uint32_t LAYER_POWER_UP = 1 << 2;

auto& collision = gameObject.add<CollisionComponent>(LAYER_PLAYER);
collision.setTriggerCollisionMask(LAYER_ENEMY | LAYER_POWER_UP);
```

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::game::physics::collision::components
@brief Collider components for collision detection.
@details This namespace contains components that define entity collision properties including bounds, layer membership, and per-frame collision state.
</p></details>

