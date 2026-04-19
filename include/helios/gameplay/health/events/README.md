# helios::gameplay::health::events

Health-related events.

## Events

| Event | Description |
|-------|-------------|
| `HealthDepletedEvent` | Emitted when an entity's health reaches zero |
| `HealthChangedEvent` | Emitted when an entity's health changes |

## HealthDepletedEvent

Published by `HealthManager` when an entity dies. 

Used by other systems (e.g., `CombatScoringSystem`, `GameObjectLifecycleSystem`) to react to entity deaths.

## HealthChangedEvent

Published when an entity's health is modified (damage or healing). Contains:

- `HealthChangeContext()` – A `HealthChangeContext` describing target, source, contact point, and amount. Used for triggering health-based reactions (e.g., hit effects, UI updates) without needing to subscribe to the full health update process.

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::gameplay::health::events
@brief Health-related events.
@details Contains event classes for health state changes and depletion.
</p></details>
