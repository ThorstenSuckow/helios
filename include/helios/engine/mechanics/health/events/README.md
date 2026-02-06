# helios::engine::mechanics::health::events

Health-related events.

## Events

| Event | Description |
|-------|-------------|
| `DeathEvent` | Emitted when an entity's health reaches zero |

## DeathEvent

Published by `HealthUpdateSystem` when an entity dies. Contains:

- `source()` - Handle of the deceased entity
- `attackContext()` - Optional combat context if death was from damage

Used by other systems (e.g., `CombatScoringSystem`) to react to entity deaths.

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::mechanics::health::events
@brief Health-related events.
@details Contains event classes for health state changes like death.
</p></details>
