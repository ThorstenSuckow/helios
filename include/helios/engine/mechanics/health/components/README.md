# helios::engine::mechanics::health::components

Health-related ECS components.

## Components

| Component | Description |
|-----------|-------------|
| `HealthComponent` | Tracks current and maximum health, provides damage/heal methods |

## HealthComponent

Manages health state for an entity:

- `takeDamage(float)` - Reduces health (clamped to 0)
- `heal(float)` - Increases health (clamped to max)
- `isAlive()` - Returns true if health > 0
- `reset()` - Restores health to maximum

Health is automatically reset when the component is acquired from a pool.

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::mechanics::health::components
@brief Health-related ECS components.
@details Contains components for tracking entity health state.
</p></details>
