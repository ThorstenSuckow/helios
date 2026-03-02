# helios::engine::mechanics::health

Health management system for game entities.

This module provides components, events, types, systems, and a manager for tracking entity health, processing damage, and handling death.

## Submodules

| Submodule | Purpose |
|-----------|---------|
| `HealthManager` | Manager that processes `ApplyDamageCommand`s in batch during flush |
| `components/` | Health-related ECS components |
| `events/` | Health-related events (update, depletion) |
| `systems/` | Systems for health processing |
| `types/` | Bitmask enums and context structs |

## Architecture

1. **HealthComponent** tracks current/maximum health and carries behaviour flags
2. **HealthManager** receives `ApplyDamageCommand`s via `submit()` and processes
   them in batch during the manager flush phase:
   - Applies damage to the target's `HealthComponent`
   - Pushes a `HealthChangedEvent` to the **phase** bus (available next phase)
   - On health depletion, evaluates `HealthDepletedBehavior` flags and optionally
     attaches a `DeadTagComponent` (flag `DeadTag`)
   - Pushes a `HealthDepletedEvent` to the **pass** bus (available after commit
     within the same pass)
3. **HealthUpdateClearSystem** resets the dirty flag after observers have run

## Key Types

- `HealthChangeBehavior` – selects the event bus for health-change reporting
- `HealthDepletedBehavior` – selects actions on health depletion (event, despawn, dead-tag)
- `HealthChangeContext` – carries target, source, contact point, and amount

## Integration

The health system integrates with:
- **Combat system** – `ApplyDamageCommand` is routed to `HealthManager`
- **Scoring system** – `HealthDepletedEvent` triggers score awards
- **Lifecycle system** – `GameObjectLifecycleSystem` reads depletion events and despawns entities
- **Spawn system** – dead entities are returned to pools via `DespawnCommand`

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::mechanics::health
@brief Health management system for game entities.
@details Provides infrastructure for tracking entity health, processing damage and healing, and handling entity death through events.
</p></details>
