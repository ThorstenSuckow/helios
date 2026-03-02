# helios::engine::mechanics::health::types

Type definitions for the health system.

## Types

| Type | Description |
|------|-------------|
| `HealthDepletedBehavior` | Bitmask flags controlling the response when health reaches zero |
| `HealthChangeContext` | Context struct describing a health modification |

## HealthDepletedBehavior

Bitmask enum with values:
- `None` – No action
- `Despawn` – Queue the entity for despawning
- `DeadTag` – Attach a DeadTagComponent

Supports bitwise operators and `hasHealthDepletedFlag()` for testing.

## HealthChangeContext

Lightweight struct carrying:
- `interactionContext` – An `InteractionContext` describing target, instigator, causer, and contact point
- `delta` – Signed health delta (negative for damage, positive for healing)

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::mechanics::health::types
@brief Type definitions for the health system.
@details Contains a bitmask enum for health depletion behaviour and a context struct for health updates.
</p></details>

