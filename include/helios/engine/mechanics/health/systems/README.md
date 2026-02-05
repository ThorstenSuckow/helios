# helios::engine::mechanics::health::systems

Systems for health processing.

## Systems

| System | Phase | Description |
|--------|-------|-------------|
| `HealthUpdateSystem` | Update | Detects dead entities, issues despawn commands, emits death events |

## HealthUpdateSystem

Processes entities with `HealthComponent` and `SpawnedByProfileComponent`:

1. Queries all entities with health components
2. Checks if `isAlive()` returns false
3. Issues `DespawnCommand` to return entity to pool
4. Publishes `DeathEvent` with optional `AttackContext`

The attack context is retrieved from `LastAttackerComponent` if present.

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::mechanics::health::systems
@brief Systems for health processing.
@details Contains systems that monitor health state and handle entity death.
</p></details>
